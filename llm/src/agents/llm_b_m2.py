import torch
from transformers import AutoModelForCausalLM, AutoTokenizer, BitsAndBytesConfig

#Model
model_id_2 = "google/gemma-3-270m"

#Device setup
has_cuda = torch.cuda.is_available()
has_mps  = torch.backends.mps.is_available()

if has_cuda:
    device = torch.device("cuda")
    compute_dtype = torch.float16
elif has_mps:
    device = torch.device("mps")
    compute_dtype = torch.float32
else:
    device = torch.device("cpu")
    compute_dtype = torch.float32

#Common model kwargs
common = dict(
    low_cpu_mem_usage=True,
    attn_implementation="eager"
)

#4-bit quantization on CUDA if available
if has_cuda:
    try:
        import bitsandbytes as bnb
        common["quantization_config"] = BitsAndBytesConfig(
            load_in_4bit=True,
            bnb_4bit_quant_type="nf4",
            bnb_4bit_use_double_quant=True,
            bnb_4bit_compute_dtype=compute_dtype,
        )
        common["device_map"] = "auto"
    except Exception:
        print("[WARN] bitsandbytes not available; using full-precision fp16 on CUDA.", flush=True)
        common["device_map"] = "auto"
else:
    common["device_map"] = None

#Tokenizer and the token automatically used if logged in via CLI
tokenizer_2 = AutoTokenizer.from_pretrained(model_id_2, use_fast=True)
if tokenizer_2.pad_token_id is None:
    tokenizer_2.pad_token = tokenizer_2.eos_token

# Model
mp_kwargs = dict(dtype=compute_dtype, **common)
model_2 = AutoModelForCausalLM.from_pretrained(model_id_2, **mp_kwargs)


if not has_cuda:
    model_2.to(device)
model_2.eval()

# Generation 
GEN_B = dict(
    max_new_tokens=32,
    do_sample=True,
    eos_token_id=tokenizer_2.eos_token_id,
    pad_token_id=tokenizer_2.pad_token_id,
)

def postprocess(text: str) -> str:
    t = text.strip()
    for sep in ["\n", ". ", "  "]:
        i = t.find(sep)
        if i > 0:
            t = t[:i]
            break
    return t.strip().strip(":").strip()

def agent2(q: str) -> str:
    prompt = f"You are a concise Q&A assistant.\n\n{q}\n"
    inputs = tokenizer_2(prompt, return_tensors="pt")

    if has_cuda:
        inputs = {k: v.to("cuda") for k, v in inputs.items()}
    elif has_mps:
        inputs = {k: v.to("mps") for k, v in inputs.items()}
    else:
        inputs = {k: v.to("cpu") for k, v in inputs.items()}

    with torch.inference_mode():
        out = model_2.generate(**inputs, **GEN_B)

    prompt_len = inputs["input_ids"].shape[1]
    result = tokenizer_2.decode(out[0][prompt_len:], skip_special_tokens=True)
    print(result)
    return postprocess(result)

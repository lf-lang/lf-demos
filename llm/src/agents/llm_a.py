# llm_a.py

import torch
from transformers import AutoModelForCausalLM, AutoTokenizer, BitsAndBytesConfig

#Model
model_id = "meta-llama/Llama-2-7b-chat-hf"


has_cuda = torch.cuda.is_available()
if not has_cuda:
    raise RuntimeError("CUDA GPU required for this configuration.")
dtype = torch.bfloat16 if has_cuda else torch.float32

#4-bit quantization
bnb_config = None
if has_cuda:
    try:
        import bitsandbytes as bnb
        bnb_config = BitsAndBytesConfig(
            load_in_4bit=True,
            bnb_4bit_quant_type="nf4",
            bnb_4bit_use_double_quant=True,
            bnb_4bit_compute_dtype=dtype,
        )
    except Exception:
        bnb_config = None

#Tokenizer and the token is automatically used if logged in via CLI
tokenizer = AutoTokenizer.from_pretrained(model_id, use_fast=True)
if tokenizer.pad_token_id is None:
    tokenizer.pad_token = tokenizer.eos_token


common = dict(
    device_map="auto" if has_cuda else None,
    torch_dtype=dtype,
    low_cpu_mem_usage=True,
)

if bnb_config is not None:
    common["quantization_config"] = bnb_config

#model
model = AutoModelForCausalLM.from_pretrained(model_id, **common)
model.eval()

#Generation 
GEN_A = dict(
    max_new_tokens=24,
    do_sample=False,
    temperature=0.1,
    eos_token_id=tokenizer.eos_token_id,
    pad_token_id=tokenizer.pad_token_id
)

#post-processing
def postprocess(text: str) -> str:
    t = text.strip()
    for sep in ["\n", ". ", "  "]:
        idx = t.find(sep)
        if idx > 0:
            t = t[:idx]
            break
    return t.strip().strip(":").strip()

#Agent 1
def agent1(q: str) -> str:
    prompt = f"You are a concise Q&A assistant.\n\n{q}\n"
    inputs = tokenizer(prompt, return_tensors="pt")
    if has_cuda:
        inputs = {k: v.to("cuda") for k, v in inputs.items()}
    with torch.no_grad():
        out = model.generate(**inputs, **GEN_A)
    prompt_len = inputs["input_ids"].shape[1]
    result = tokenizer.decode(out[0][prompt_len:], skip_special_tokens=True)
    print(result)
    return postprocess(result)

# llm_b.py — Agent 2 (70B)

import torch
from transformers import AutoModelForCausalLM, AutoTokenizer, BitsAndBytesConfig

# <<< put your token here >>>
hf_auth = "add token here"

# Model to be chosen to act as an agent
model_id_2 = "meta-llama/Llama-2-70b-chat-hf"

# Require GPU (GPU-only)
has_cuda = torch.cuda.is_available()
if not has_cuda:
    raise RuntimeError("CUDA GPU required for this configuration.")
dtype = torch.bfloat16 if has_cuda else torch.float32

# 4-bit quantization
bnb_config = None
if has_cuda:
    try:
        import bitsandbytes as bnb  # noqa: F401
        bnb_config = BitsAndBytesConfig(
            load_in_4bit=True,
            bnb_4bit_quant_type="nf4",
            bnb_4bit_use_double_quant=True,
            bnb_4bit_compute_dtype=dtype,
        )
    except Exception:
        bnb_config = None

# Tokenizer
tokenizer_2 = AutoTokenizer.from_pretrained(model_id_2, token=hf_auth, use_fast=True)
if tokenizer_2.pad_token_id is None:
    tokenizer_2.pad_token = tokenizer_2.eos_token

# Shared kwargs
common = dict(
    device_map="auto" if has_cuda else None,
    dtype=dtype,
    low_cpu_mem_usage=True,
)
if bnb_config is not None:
    common["quantization_config"] = bnb_config

# Model
model_2 = AutoModelForCausalLM.from_pretrained(model_id_2, token=hf_auth, **common)
model_2.eval()

# Generation args
GEN_B = dict(
    max_new_tokens=24, do_sample=False, temperature=0.1,
    eos_token_id=tokenizer_2.eos_token_id, pad_token_id=tokenizer_2.pad_token_id
)

# One-line postprocess
def postprocess(text: str) -> str:
    t = text.strip()
    for sep in ["\n", ". ", "  "]:
        idx = t.find(sep)
        if idx > 0:
            t = t[:idx]
            break
    return t.strip().strip(":").strip()

# Agent 2 entrypoint
def agent2(q: str) -> str:
    prompt = f"You are a concise Q&A assistant.\n\n{q}\n"
    inputs = tokenizer_2(prompt, return_tensors="pt")
    if has_cuda:
        inputs = {k: v.to("cuda") for k, v in inputs.items()}
    with torch.no_grad():
        out = model_2.generate(**inputs, **GEN_B)
    prompt_len = inputs["input_ids"].shape[1]
    result = tokenizer_2.decode(out[0][prompt_len:], skip_special_tokens=True)
    print(result)
    return postprocess(result)
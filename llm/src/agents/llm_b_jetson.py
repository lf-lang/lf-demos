import torch
from transformers import AutoModelForCausalLM, AutoTokenizer


# Model ID
model_id = "meta-llama/Llama-3.2-1B"

# Check GPU availability
has_cuda = torch.cuda.is_available()
device = torch.device("cuda" if has_cuda else "cpu")
compute_dtype = torch.float16 if has_cuda else torch.float32


common = dict(
    low_cpu_mem_usage=True,
    attn_implementation="eager",
)

#Load tokenizer and the token automatically used from CLI login
tokenizer = AutoTokenizer.from_pretrained(model_id, use_fast=True)
if tokenizer.pad_token_id is None:
    tokenizer.pad_token = tokenizer.eos_token

#Load model
mp_kwargs = dict(torch_dtype=compute_dtype, **common)
model = AutoModelForCausalLM.from_pretrained(model_id, **mp_kwargs)
model.to(device)
model.eval()

#Generation 
GEN = dict(
    max_new_tokens=64,
    do_sample=True,
    temperature=0.7,
    top_p=0.95,
    eos_token_id=tokenizer.eos_token_id,
    pad_token_id=tokenizer.pad_token_id,
)

#Agent 2
def agent2(q: str) -> str:
    prompt = f"You are a concise Q&A assistant.\n\n{q}\n"
    inputs = tokenizer(prompt, return_tensors="pt").to(device)
    with torch.inference_mode():
        out = model.generate(**inputs, **GEN)
    gen = out[0, inputs["input_ids"].shape[1]:]
    return tokenizer.decode(gen, skip_special_tokens=True).strip()

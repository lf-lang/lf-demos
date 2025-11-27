### Import Libraries 
import transformers
import torch
from transformers import AutoModelForCausalLM, AutoTokenizer, BitsAndBytesConfig
from torch import cuda, bfloat16


### Model to be chosen to act as an agent 
model_id = "meta-llama/Llama-2-7b-chat-hf"  
model_id_2 = "meta-llama/Llama-2-70b-chat-hf" 

### To check if there is GPU and convert it into float 16
has_cuda = torch.cuda.is_available()
dtype = torch.bfloat16 if has_cuda else torch.float32   

### To convert the model into 4bit quantization 
bnb_config = None
### if there is cuda then the model is converted to 4bit quantization
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

### calling pre-trained tokenizer
tokenizer   = AutoTokenizer.from_pretrained(model_id,   use_fast=True)
tokenizer_2 = AutoTokenizer.from_pretrained(model_id_2, use_fast=True)
for tok in (tokenizer, tokenizer_2):
    if tok.pad_token_id is None:
        tok.pad_token = tok.eos_token

### since both the models have same device map and using 4bit quantization for both
common = dict(
    device_map="auto" if has_cuda else None,
    torch_dtype=dtype,            # Changed from dtype=dtype (correct arg name)             
    low_cpu_mem_usage=True,
)
if bnb_config is not None:
    common["quantization_config"] = bnb_config

### calling pre-trained model
model   = AutoModelForCausalLM.from_pretrained(model_id,   **common)
model_2 = AutoModelForCausalLM.from_pretrained(model_id_2, **common)
model.eval(); model_2.eval()


### arguments for both the models 
GEN_A = dict(max_new_tokens=24, do_sample=False, temperature=0.1,
             eos_token_id=tokenizer.eos_token_id, pad_token_id=tokenizer.pad_token_id)
GEN_B = dict(max_new_tokens=24, do_sample=False, temperature=0.1,
             eos_token_id=tokenizer_2.eos_token_id, pad_token_id=tokenizer_2.pad_token_id)

###to resturn only one line answers
def postprocess(text: str) -> str:
    t = text.strip()
    for sep in ["\n", ". ", "  "]:
        idx = t.find(sep)
        if idx > 0:
            t = t[:idx]
            break
    return t.strip().strip(":").strip()

###Calling agent1 from .lf code
def agent1(q: str) -> str:
    prompt = f"You are a concise Q&A assistant.\n\n{q}\n"
    inputs = tokenizer(prompt, return_tensors="pt")
    if has_cuda: inputs = {k: v.to("cuda") for k, v in inputs.items()}
    with torch.no_grad():
        out = model.generate(**inputs, **GEN_A)
    prompt_len = inputs["input_ids"].shape[1]
    result = tokenizer.decode(out[0][prompt_len:], skip_special_tokens=True)
    return postprocess(result)

###Calling agent2 from .lf code
def agent2(q: str) -> str:
    prompt = f"You are a concise Q&A assistant.\n\n{q}\n"
    inputs = tokenizer_2(prompt, return_tensors="pt")
    if has_cuda: inputs = {k: v.to("cuda") for k, v in inputs.items()}
    with torch.no_grad():
        out = model_2.generate(**inputs, **GEN_B)
    prompt_len = inputs["input_ids"].shape[1]
    result = tokenizer_2.decode(out[0][prompt_len:], skip_special_tokens=True)
    return postprocess(result)

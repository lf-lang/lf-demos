### Import Libraries 
import transformers
import torch
from transformers import AutoModelForCausalLM, AutoTokenizer, BitsAndBytesConfig
from torch import cuda, bfloat16

### Add Your hugging face token here 
hf_auth = "Add here"

### Model to be chosen to act as an agent 
model_id = "meta-llama/Llama-2-7b-chat-hf"  

### To check if there is GPU
has_cuda = torch.cuda.is_available()

### To convert the model into 4bit quantization 
bnb_config = None
if has_cuda:
    try:
        import bitsandbytes as bnb  
        bnb_config = BitsAndBytesConfig(
            load_in_4bit=True,
            bnb_4bit_quant_type="nf4",
            bnb_4bit_use_double_quant=True,
            bnb_4bit_compute_dtype=torch.bfloat16,  
        )
    except Exception:
        bnb_config = None  

### calling pre-trained tokenizer
tokenizer = AutoTokenizer.from_pretrained(model_id, token=hf_auth, use_fast=True)


### calling pre-trained model
model = AutoModelForCausalLM.from_pretrained(
    model_id,
    token=hf_auth,
    device_map="auto" if has_cuda else None,
    torch_dtype=torch.bfloat16 if has_cuda else torch.float32,
    quantization_config=bnb_config,              
    low_cpu_mem_usage=True,
)

model.eval()

### agent 1
def agent1(a):
    user_query = a

    prompt = f"You are a helpful assistant.\n\n{user_query}\n"

    inputs = tokenizer(prompt, return_tensors="pt")

    if has_cuda:
        inputs = {k: v.to("cuda") for k, v in inputs.items()}

    with torch.no_grad():
        outputs = model.generate(
            **inputs,
            max_new_tokens=100,
            do_sample=True,
            temperature=0.3,
        )

    gen_tokens = outputs[0]
    prompt_len = inputs["input_ids"].shape[1]
    response = tokenizer.decode(gen_tokens[prompt_len:], skip_special_tokens=True)

    print("LLM A response:", response)
    return response

### agent 2 
def agent2(b):
    user_query = b

    prompt = f"Just summarize what the agent1 said: \n\n{user_query}\n\n"

    inputs = tokenizer(prompt, return_tensors="pt")

    if has_cuda:
        inputs = {k: v.to("cuda") for k, v in inputs.items()}

    with torch.no_grad():
        outputs = model.generate(
            **inputs,
            max_new_tokens=100,
            do_sample=True,
            temperature=0.3,
        )

    gen_tokens = outputs[0]
    prompt_len = inputs["input_ids"].shape[1]
    response = tokenizer.decode(gen_tokens[prompt_len:], skip_special_tokens=True)
    print("LLM B response:", response)
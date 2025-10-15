# LLM Demo (Federated Execution)

# Overview
This is a quiz-style game between two LLM agents using federated execution. For each user question asked to the Judge, both agents answer in parallel. The Judge announces whichever answer arrives first (or a timeout if neither responds within 60 sec), and prints per-question elapsed logical and physical times. There are three federates (federate__llma, federate__llmb, federate__j) and an RTI.

# Pre-requisites 

You need Python installed, as llm_a.py, llm_b.py, llm_b_m2.py and llm_b_jetson.py are written in Python. # any version >= 3.10

## Library Dependencies
To run this project, there are dependencies required which are in [requirements.txt](requirements.txt) file. The model used in this repository has been quantized using 4-bit precision (bnb_4bit) and relies on bitsandbytes for efficient matrix operations and memory optimization. So specific versions of bitsandbytes, torch, and torchvision are mandatory for compatibility. 
While newer versions of other dependencies may work, the specific versions listed below have been tested and are recommended for optimal performance.
It is highly recommended to create a Python virtual environment or a Conda environment to manage dependencies. 

## System Requirements  

To ensure optimal performance, the following hardware and software requirements are utilized. \
**Note:** To replicate this model, you can use any equivalent hardware that meets the computational requirements.

### Hardware Requirements   
- **GPU**: NVIDIA RTX A6000  

### Software Requirements  
- **Python** (Ensure Python is installed)  
- **CUDA Version**: 12.8  
- **NVIDIA-SMI**: For monitoring GPU performance and memory utilization  

### Model Dependencies  
- **Pre-trained Models**:  [meta-llama/Llama-2-7b-chat-hf](https://huggingface.co/meta-llama/Llama-2-7b-chat-hf)  [meta-llama/Llama-2-70b-chat-hf](https://huggingface.co/meta-llama/Llama-2-70b-chat-hf) 
**Note:** Please access and use the pre-trained models, authentication keys must be obtained from the [Hugging Face repository](https://huggingface.co/settings/tokens). Ensure you have a valid API token and configure authentication.

Make sure the environment is properly configured to use CUDA for optimal GPU acceleration.

# Files and directories in this repository
  - **`llm_base_class_federate.lf`** - Contains the base reactors LlmA, LlmB and Judge.
  - **`llm_game_federated.lf`** - Lingua Franca program that defines the quiz game as federated execution.

# Execution Workflow 

### Step 1: 
To compile this specify the RTI host by specifying an IP address here:
```
federated reactor llm_game_federated at 10.xxx.xxx.xx {
}
```

Run the **`llm_game_federated.lf`**.  

**Note:**  
- Ensure that you specify the correct file paths

Run the following commands:  

```
lfc src/federated_execution/llm_game_federated.lf
```

### Step 2: Run the binary file and input the quiz question
Run the following command:  

```
cd fed-gen/llm_game_federated/
```

In the first terminal run:
```
./bin/RTI -n 3

```
In the second terminal run:
```
./bin/federate__j

```
In the third terminal run:
```
./bin/federate__llma

```
In the fourth terminal run:
```
./bin/federate__llmb

```

The system will ask for entering the quiz question which is to be obtained from the keyboard input.

Example output printed on the terminal:
 
<pre>

--------------------------------------------------
---- System clock resolution: 1 nsec
---- Start execution on Fri Sep 19 10:46:31 2025 ---- plus 772215861 nanoseconds
Enter the quiz question
What is the capital of South Korea?
Query: What is the capital of South Korea?

waiting...

Winner: LLM-B | logical 1184 ms | physical 1184 ms
Answer: Seoul.
--------------------------------------------------

</pre>

# Contributors

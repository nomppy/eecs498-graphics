# Diffusion 

### Task3: Please report the time you need for DDPM and DDIM respectively and briefly talk about the pros and cons of using DDIM as the scheduler over DDPM. One sentence response is adequate. 
DDPM takes around 10 minutes, DDIM around 17 seconds. DDIM is much faster in sampling but is deterministic with less sample diversity.

### Task5: Question: What is the extra time cost for applying the DPS compared to DDIM? What will happen if we set fewer timesteps than usual (look at TODO in execute_DPS.py)? Please run the experiment in different timesteps and share your findings. The response can be as short as one sentence. Sticking some figures is recommended. 
DPS takes about 3.8 seconds per image, while no DPS takes around 1.9 seconds, so DPS takes around twice as long for the same number of diffusion timesteps. In addition, setting fewer timesteps in DPS results in output not fitting measurements perfectly. So even though a realistic image is generated through the DDIM process, it takes many more timesteps for DPS to "push" the generation towards the observed measurement. 

50 steps vs 1000 steps of DPS
![DPS_50_steps](50_DPS_results/DPS_generated_image0.png)
![DPS_1000_steps](submit_DPS_results/DPS_generated_image0.png)

### Task (optional extra credit): Checkout lines 56-59 of training/loss.py. Think about why we only need to modify one part and it is enough to handle three cases shown in Fig. 5. Share your thoughts here. 
Because we want every input to the discriminator to be augmented (this is what Figure 5. shows), adding it in when we run discriminator is enough. The previously proposed cases in the DiffAug paper of either augmenting reals only, generated only, etc. are actually more complicated implementation wise.
Side note: I think `Pillow` may be not included in `requirements.txt`, I had to install it manually

# GAN

### Task 2: Please explain what is the difference between Ez∼p(z) term in Eq. 2 and Eq. 1? And why there is such a difference between the Generator and the Discriminator? Briefly explain the difference between the objective of the Generator and the Discriminator loss function is adequate. Write your response in a PDF or TXT file and submit it. 

The objective of generator (G) is to get the discriminator to return a high confidence that G(z) is real. But discriminator wants to return low confidence that G(z) is real, which is why they have opposite signs in the $E_{z\sim p(z)}$ term. 

### Extra credit: Checkout lines 56-59 of training/loss.py. Think about why we only need to modify one part and it is enough to handle three cases shown in Fig. 5. Share your thoughts here. 
The results of the DiffAug paper says that it is not enough to augment either only the real images, the generated images, or only when we calculate the discriminator loss. 
In fact we want to apply the same augmentation function $T$ every time we run the discriminator, hence it is enough to just apply it in the `run_D` function.

### Task (optional extra credits): Despite the two regularization techniques we have shown above, what are some other augmentation or regularization techniques that are functional in GAN? Please search for the literature online and list three or more here. 
1. R2 regularziation: penalize gradient of discriminator output wrt fake data
2. Apply weight dropout to generator or discriminator to prevent overfitting
3. Inject noise in feature maps of the discriminator (also decrease sensitivity to small changes)
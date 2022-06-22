# Pixel count
 Counts the number of pixels of one color in a PNG image encoded in RBG.
 Preferably with just one IDAT chunk.

 Built with
 
    gcc pixel-count.c -lz [-lm]
    
 lm flag is optional, you might or might not need to link the math library
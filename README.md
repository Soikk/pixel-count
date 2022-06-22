
# Pixel count
 Counts the number of pixels of one color in a PNG image encoded in RBG.
 Preferably with just one IDAT chunk.

 Built with
 
    gcc pixel-count.c -lz [-lm]
    
 lm flag is optional, you might or might not need to link the math library


 Resources (in no particular order):
 - https://www.w3.org/TR/2003/REC-PNG-20031110/
 - https://www.libpng.org/pub/png/spec/1.2/
 - https://pyokagan.name/blog/2019-10-14-png/
 - https://hexed.it/
 - https://www.nayuki.io/page/png-file-chunk-inspector
 - https://github.com/madler/zlib
 - https://stackoverflow.com/questions/42934720/understanding-the-zlib-header-cmf-cm-cinfo-flg-fdict-dictid-flevel-rfc
 - Tons of other stackoverflow answers
 
Checked solution with https://townsean.github.io/canvas-pixel-color-counter/

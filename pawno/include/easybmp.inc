native GetImageRAtPos(const BMP24BIT_FileName[],BMP_PosX,BMP_PosY);
native GetImageGAtPos(const BMP24BIT_FileName[],BMP_PosX,BMP_PosY);
native GetImageBAtPos(const BMP24BIT_FileName[],BMP_PosX,BMP_PosY);
native GetImageWidth(const BMP24BIT_FileName[]);
native GetImageHeight(const BMP24BIT_FileName[]);
native SetPixelRGBAAtPos(const BMP24BIT_FileName[],BMP_PosX,BMP_PosY,red,green,blue,alpha=0);
native SetImageSize(const BMP24BIT_FileName[],BMP_PosX,BMP_PosY);
//native GetImageRGBAtPos(const BMP24BIT_FileName[],BMP_PosX,BMP_PosY,&DecimalRed,&DecimalGreen,&DecimalBlue);//sorry I don't know how to set adresses from a plugin...

//fast image processing
native FImageOpen(const BMP24BIT_FileName[]);
native FImageSave();// only save the image when you have used FSet*
native FGetImageRAtPos(BMP_PosX,BMP_PosY);
native FGetImageGAtPos(BMP_PosX,BMP_PosY);
native FGetImageBAtPos(BMP_PosX,BMP_PosY);
native FGetImageWidth();
native FGetImageHeight();
native FSetPixelRGBAAtPos(BMP_PosX,BMP_PosY,red,green,blue,alpha=0);
native FSetImageSize(BMP_PosX,BMP_PosY);
//native FGetImageRGBAtPos(const BMP_PosX,BMP_PosY,&DecimalRed,&DecimalGreen,&DecimalBlue);//sorry I don't know how to set adresses from a plugin...


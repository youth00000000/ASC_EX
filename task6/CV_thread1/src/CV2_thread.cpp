#include "CV2.h"
double CV2::grepBitmap()
{
    /*
        process the grep picture
    */
    double t1, t2;
    _pictureCol = pictureCol;
    _pictureRow = pictureRow;

    //创建一个空白图像板
    createBitmap(); // creat the blank picture board

    //定义RGB类型指针数组,指向图像点
    RGB *piont[pictureRow], *_piont[pictureRow];

    //指向每一行的起始地址
    /*-------the primary of piont and _piont----------*/
    for (int i = 0; i < pictureRow; i++)
    {
        piont[i] = (RGB *)((char *)priex + i * stride);    // piont
        _piont[i] = (RGB *)((char *)_priex + i * _stride); //_piont
    }
    /*-----------------------------------------------*/

    t1 = omp_get_wtime();
    //对_point指向的图像进行灰度化处理
    for (int i = 0; i < _pictureRow; i++)
    {
        for (int j = 0; j < _pictureCol; j++)
        {
            BYTE base = (BYTE)(piont[i][j].Blue * 0.33 + piont[i][j].Green * 0.33 + piont[i][j].Red * 0.33); // the vital
            _piont[i][j].Blue = base;
            _piont[i][j].Green = base;
            _piont[i][j].Red = base;
        }
    }
    t2 = omp_get_wtime();
    std::cout << std::endl;
    std::cout << "//*****************************//" << std::endl;
    //计算灰度化用时
    std::cout << "// grep is: " << (t2 - t1) << "//" << std::endl;
    std::cout << "//*****************************//" << std::endl;
    std::cout << std::endl;
    return (t2 - t1);
}
double CV2::ac_grepBitmap() 
{
    /*
        process the grep picture
    */
    double t1, t2;
    _pictureCol = pictureCol;
    _pictureRow = pictureRow;

    //创建一个空白图像板
    createBitmap(); // creat the blank picture board

    //定义RGB类型指针数组,指向图像点
    RGB *piont[pictureRow], *_piont[pictureRow];

    size_t num_thread = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(num_thread);
    //指向每一行的起始地址
    /*-------the primary of piont and _piont----------*/

    for (int t = 0; t < num_thread; t++)
    {
        threads[t] = std::thread([&, t]()
        {
            for (int i = t; i < pictureRow; i += num_thread)
            {
                piont[i] = (RGB *)((char *)priex + i * stride);    // piont
                _piont[i] = (RGB *)((char *)_priex + i * _stride); //_piont
            }
        });
    }

    for (int t = 0; t < num_thread; t++)
    {
        threads[t].join();
    }

    threads.clear();
    threads.resize(num_thread);

    t1 = omp_get_wtime();
    
    for (int t = 0; t < num_thread; t++)
    {
        threads[t] = std::thread([&]()
        {
            for (int i = t; i < _pictureRow; i += num_thread)
            {
                for (int j = 0; j < _pictureCol; j++)
                {
                    BYTE base = (BYTE)(piont[i][j].Blue * 0.33 + piont[i][j].Green * 0.33 + piont[i][j].Red * 0.33); // the vital
                    _piont[i][j].Blue = base;
                    _piont[i][j].Green = base;
                    _piont[i][j].Red = base;
                }
            }
        });
    }

    
    for (int t = 0; t < num_thread; t++)
    {
        threads[t].join();
    }
    t2 = omp_get_wtime();
    std::cout << std::endl;
    std::cout << "//*****************************//" << std::endl;
    //计算优化后灰度化用时
    std::cout << "// ac_grep is: " << (t2 - t1) << "//" << std::endl;
    std::cout << "//*****************************//" << std::endl;
    std::cout << std::endl;

    return (t2 - t1);
}
double CV2::binaryzationBitmap(int value)
{
    /*
        process the piture's binaryzation
    */
    double t1, t2;
    _pictureCol = pictureCol;
    _pictureRow = pictureRow;
    RGB *piont[pictureRow], *_piont[pictureRow];
    createBitmap(); // creat the blank picture board

    /*-------the primary of piont and _piont----------*/
    for (int i = 0; i < pictureRow; i++)
    {
        piont[i] = (RGB *)((char *)priex + i * stride);    // piont
        _piont[i] = (RGB *)((char *)_priex + i * _stride); //_piont
    }
    /*-----------------------------------------------*/
    t1 = omp_get_wtime();
    for (int i = 0; i < pictureRow; i++)
    {
        for (int j = 0; j < pictureCol; j++)
        {
            BYTE base = (BYTE)(piont[i][j].Blue * 0.33 + piont[i][j].Green * 0.33 + piont[i][j].Red * 0.33);
            //二值化处理
            //如果像素值大于阈值,则将其置为白色,否则置为黑色
            if (base > value)
                base = WHITE; // the vital
            else
                base = BLACK;
            _piont[i][j].Blue = base;
            _piont[i][j].Green = base;
            _piont[i][j].Red = base;
        }
    }
    t2 = omp_get_wtime();
    std::cout << std::endl;
    std::cout << "//*****************************//" << std::endl;
    //计算二值化用时
    std::cout << "// binaryzation is: " << (t2 - t1) << "//" << std::endl;
    std::cout << "//*****************************//" << std::endl;
    std::cout << std::endl;

    return (t2 - t1);
}
double CV2::ac_binaryzationBitmap(int value) 
{
    /*
        process the piture's binaryzation
    */
    double t1, t2;
    _pictureCol = pictureCol;
    _pictureRow = pictureRow;
    RGB *piont[pictureRow], *_piont[pictureRow];
    createBitmap(); // creat the blank picture board
    
    size_t num_thread = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(num_thread);

    for (int t = 0; t < num_thread; t++)
    {
        threads[t] = std::thread([&, t]()
        {
            for (int i = t; i < pictureRow; i += num_thread)
            {
                piont[i] = (RGB *)((char *)priex + i * stride);    // piont
                _piont[i] = (RGB *)((char *)_priex + i * _stride); //_piont
            }
        });
    }

    for (int t = 0; t < num_thread; t++)
    {
        threads[t].join();
    }

    threads.clear();
    threads.resize(num_thread);

    t1 = omp_get_wtime();

    for (int t = 0; t < num_thread; t++)
    {
        threads[t] = std::thread([&]()
        {
            for (int i = t; i < _pictureRow; i += num_thread)
            {
                for (int j = 0; j < _pictureCol; j++)
                {
                    BYTE base = (BYTE)(piont[i][j].Blue * 0.33 + piont[i][j].Green * 0.33 + piont[i][j].Red * 0.33);
                    //二值化处理
                    //如果像素值大于阈值,则将其置为白色,否则置为黑色
                    if (base > value)
                        base = WHITE; // the vital
                    else
                        base = BLACK;
                    _piont[i][j].Blue = base;
                    _piont[i][j].Green = base;
                    _piont[i][j].Red = base;
                }
            }
        });
    }

    for (int t = 0; t < num_thread; t++)
    {
        threads[t].join();
    }
    
    t2 = omp_get_wtime();
    std::cout << std::endl;
    std::cout << "//*****************************//" << std::endl;
    //计算优化后二值化用时
    std::cout << "// ac_binaryzation is: " << (t2 - t1) << "//" << std::endl;
    std::cout << "//*****************************//" << std::endl;
    std::cout << std::endl;

    return (t2 - t1);
}
double CV2::segmentBitmap(int beginCol, int beginRow, int _col, int _row)
{
    /*
        to segment your picture
    */
    double t1, t2;
    _pictureRow = _row;
    _pictureCol = _col;
    createBitmap(); // creat the blank picture board
    RGB *piont[pictureRow], *_piont[pictureRow];
    for (int i = 0; i < pictureRow; i++)
    {
        piont[i] = (RGB *)((char *)priex + i * stride);    // piont
        _piont[i] = (RGB *)((char *)_priex + i * _stride); //_piont
    }
    int Clen = beginCol + _pictureCol;
    int Rlen = beginRow + _pictureRow;
    t1 = omp_get_wtime();
    for (int i = beginRow; i < Rlen; i++)
    {
        for (int j = beginCol; j < Clen; j++)
        {
            _piont[i - beginRow][j - beginCol] = piont[i][j]; // the vital
        }
    }
    t2 = omp_get_wtime();
    std::cout << std::endl;
    std::cout << "//*****************************//" << std::endl;
    std::cout << "// segment is: " << (t2 - t1) << "//" << std::endl;
    std::cout << "//*****************************//" << std::endl;
    std::cout << std::endl;

    return (t2 - t1);
}
double CV2::ac_segmentBitmap(int beginCol, int beginRow, int _col, int _row) 
{
    /*
        to segment your picture
    */
    double t1, t2;
    _pictureRow = _row;
    _pictureCol = _col;
    createBitmap(); // creat the blank picture board
    RGB *piont[pictureRow], *_piont[pictureRow];
    
    size_t num_thread = 4;
    std::vector<std::thread> threads(num_thread);

    for (int t = 0; t < num_thread; t++)
    {
        threads[t] = std::thread([&, t]()
        {
            for (int i = t; i < pictureRow; i += num_thread)
            {
                piont[i] = (RGB *)((char *)priex + i * stride);    // piont
                _piont[i] = (RGB *)((char *)_priex + i * _stride); //_piont
            }
        });
    }
    
    for (int t = 0; t < num_thread; t++)
    {
        threads[t].join();
    }

    threads.clear();
    threads.resize(num_thread);

    int Clen = beginCol + _pictureCol;
    int Rlen = beginRow + _pictureRow;
    t1 = omp_get_wtime();
    
    for (int t = 0; t < num_thread; t++)
    {
        threads[t] = std::thread([&]()
        {
            for (int i = beginRow + t; i < Rlen; i += num_thread)
            {
                for (int j = beginCol; j < Clen; j++)
                {
                    _piont[i - beginRow][j - beginCol] = piont[i][j]; // the vital
                }
            }
        });
    }

    for (int t = 0; t < num_thread; t++)
    {
        threads[t].join();
    }

    t2 = omp_get_wtime();
    std::cout << std::endl;
    std::cout << "//*****************************//" << std::endl;
    std::cout << "// ac_segment is: " << (t2 - t1) << "//" << std::endl;
    std::cout << "//*****************************//" << std::endl;
    std::cout << std::endl;

    return (t2 - t1);
}
void CV2::createBitmap()
{
    /*
        to creat the blank picture board
    */
    _Tag = *Tag;
    _Head = *Head;
    _Tag.biWidth = _pictureCol;                                      // change of the picture col
    _Tag.biHeight = _pictureRow;                                     // change of the picture row
    _Head.bfSize = Head->bfOffBits + _pictureRow * stride;           // change of the head and tag
    out = malloc(_Head.bfSize);                                      // call some room of the new picture
    _stride = (((_pictureCol * (Tag->biBitCount) + 31) & ~31) >> 3); // change of the new stride
    _pictureSize = _Head.bfSize;
    *(BITMAPFILE *)out = _Head;                      // introduce the new Head
    *(BITMAPINFOR *)((char *)out + 14) = _Tag;       // introduce the new Tag
    _priex = (RGB *)((char *)out + Head->bfOffBits); // locate the out(pionter)
}
CV2::CV2(FILE *file)
{
    /*
        jude the picture whether exsit and get the basic information of the picture
    */
    if (file == NULL)
        std::cout << "The file does not exit" << std::endl;
    file_in = file;
    fseek(file, 0, SEEK_END);
    pictureSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    bitmap = (void *)malloc(pictureSize);
    size_t size1 = sizeof(FileHeadBitmap);
    size_t size2 = sizeof(BITMAPINFOR);
    fread(bitmap, pictureSize, 1, file);                             // put your picture to your memory
    Head = (FileHeadBitmap *)bitmap;                                 // creat the Head
    Tag = (BITMAPINFOR *)(Head + 1);                                 // creat the Tag
    priex = (RGB *)((char *)Head + (Head->bfOffBits));               // creat the priex
    stride = (((Tag->biWidth * (Tag->biBitCount) + 31) & ~31) >> 3); // 四字节对齐 // the size of stride
    pictureRow = Tag->biHeight;                                      // the size of row;
    pictureCol = Tag->biWidth;                                       // the size of col
    std::cout << "//*****************************//" << std::endl;
    std::cout << "//      Get the picture        //" << std::endl; // make sure the picture is gotten
    std::cout << "//*****************************//" << std::endl;
}
CV2::~CV2()
{
    // release the the old and new picture in your memory
    // and release the file pionter
    std::cout << "//*****************************//" << std::endl;
    std::cout << "//The room of memory is release//" << std::endl;
    std::cout << "//*****************************//" << std::endl;
    free(bitmap);
    free(out);
    fclose(file_in);
    fclose(file_out);
}
void CV2::readBitmap(FILE *file)
{
    /*
        put out your picture that has been processed
    */
    file_out = file;
    fwrite(out, _pictureSize, 1, file);
}
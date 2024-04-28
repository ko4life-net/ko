//////////////////////////////////////////////////////////////////////
//
// Jpeg.h: interface for the CJpeg class.
//
// Main operation: Decode the JPEG file and read it or write it to disk.
//
// Original author: unknown
// Modified by: Lee Hyun-seok
// E-Mail : evaasuka1004@hotmail.com
//////////////////////////////////////////////////////////////////////

#pragma once

struct SET {
    BYTE C[3]; //Y, Cb, Cr components
};

struct DQT {
    BYTE Q[64]; // Quantization Table Values
};

struct DHT {
    BOOL   Flag;        // flag indicating whether it was used or not
    int    Num;         // Number of Huffman codes
    WORD * HUFFCODE;    // Huffman code
    BYTE * HUFFSIZE;    // Length of Huffman code
    BYTE * HUFFVAL;     // Value indicated by Huffman code
    WORD   MAXCODE[17]; // Largest code of that length
    WORD   MINCODE[17]; // smallest code of that length
    int    VALPTR[17];  // Index where code of that length starts
    int *  PT;          // Pointer to quickly find INDEX by VALUE
};

struct FRAMEHEADER {
    WORD Y;     // height of image
    WORD X;     //width of image
    BYTE Nf;    // number of components
    BYTE C[3];  // component ID
    BYTE H[3];  // Horizontal Sampling Factor of the component
    BYTE V[3];  // Vertical Sampling Factor of the component
    BYTE Tq[3]; // Quantization table number used for the component
};

struct SCANHEADER {
    BYTE Ns;    // number of components
    BYTE Cs[3]; // component ID
    BYTE Td[3]; // DC Huffman Table number of the component
    BYTE Ta[3]; // AC Huffman Table number of the component
    BYTE Ss;
    BYTE Se;
    BYTE Ah;
    BYTE Al;
};

class CJpeg {
  public:
    // Functions for loading JPEG File //
    void  LoadJPG(LPCSTR FileName);  // Function to load JPEG File
    void  FindSOI();                 // Function to find the Start of Image marker
    void  FindDQT();                 // Function to find the Quantization Table and set it in the structure
    void  FindDHT();                 // Function to find the Huffman Table and set it in the structure
    void  FindSOF();                 // Function to find the Frame Header and set it in the structure
    void  FindSOS();                 // Function to find the Scan Header and set it in the structure
    void  FindETC();                 // Load DRI (Define Restart Interval)
    void  Decode();                  // Set information for decoding and start decoding
    void  DecodeMCU(int mx, int my); // Function to decode the MCU block
    void  DecodeDU(int N);           // Function to decode 8x8 Data Unit
    void  IDCT();                    // Function to perform inverse DCT
    void  Zigzag();                  // Function to restore DU in Zigzag order to its original state
    void  DecodeAC(int Th);          // Function to decode AC component during DU
    void  DecodeDC(int Th);          // Function to decode DC component during DU
    short Extend(WORD V, BYTE T);    // extend V to fit category T
    WORD  Receive(BYTE SSSS);        // Function to read SSSS bits from the buffer
    BYTE  hDecode(int Th);           // The part that decodes the Huffman code
    BYTE  NextByte();                // Function to read the next 1 byte from the buffer
    WORD  NextBit();                 // Function to read the next 1 bit from the buffer
    void  ConvertYUV2RGB();          // At the same time as changing the color model of the decoded data
                                     // Function to convert to be compatible with bitmap

    // Functions for saving JPEG File //
    void SaveJPG(LPCSTR FileName, int Width, int Height, BYTE * pp); // Function to save JPEG file

    void PutSOI(HANDLE hFile);                        // Insert Start of Image marker
    void PutDQT(HANDLE hFile);                        // Insert Quantizatino Table
    void PutDHT(HANDLE hFile);                        // Insert Huffmann Table
    void PutSOF(HANDLE hFile, int Width, int Height); // Insert FrameHeader
    void PutSOS(HANDLE hFile);                        // Insert ScanHeader
    void PutEOI(HANDLE hFile);                        // Insert End of Image marker

    void ShotBit(BYTE Bit);               // Function to store 1 bit in the buffer
    void ChargeCode(WORD Code, int Size); // Function to store the code in the buffer as long as Size (from the bottom)
    void EncodeDU(short * pos, BOOL Flag, int bWidth); // Function to encode 8x8 Block
    void hEncode(int bWidth, int bHeight);             // Huffman encoding part
    BYTE GetCategory(short V);                         // Function to find the category of a given value
    void Zigzag2();                                    // Function to create in Zigzag order
    void DCT(short * pos, int bWidth, BOOL Flag);      // Function for Zigzag and Quantization after DCT

    int GetHeight(); // Function that returns the height of the image
    int GetWidth();  // Function that returns the width of the image

    WORD   Ri;        // Restart Interval
    int    m_rWidth;  // actual width of the image
    int    m_rHeight; // actual height of the image
    BYTE * pByte;     // Used in NextByte() function

    int cnt; // Counter used when calculating in bit units

    short * Y;  // Y Buffer used when saving
    short * Cb; // Cb Buffer used when saving
    short * Cr; // Cr Buffer used when saving

    SET * MCU; //MCU unit block

    BYTE Hmax; // Maximum Horizontal Sampling Factor
    BYTE Vmax; // Maximum Vertical Sampling Factor

    BYTE *      m_pData;     // image buffer
    BYTE *      m_pBuf;      // buffer
    int         m_Index;     // Index indicating the location of the buffer
    DQT         TbQ[20];     // Quantization Table
    DHT         TbH[20];     // Huffman Table
    short       ZZ[64];      // Array containing 8x8 Block information
    FRAMEHEADER FrameHeader; // FrameHeader structure
    SCANHEADER  ScanHeader;  //ScanHeader structure
    short       PrevDC[3];   // Predictor of DC component

    CJpeg();
    virtual ~CJpeg();
};

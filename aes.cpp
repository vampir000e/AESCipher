#include "aes.h"
#include "string.h"


AES::AES(unsigned char* key)
{
    unsigned char sBox[] =   //置换表
    { /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
        0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76, /*0*/
        0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0, /*1*/
        0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15, /*2*/
        0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75, /*3*/
        0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84, /*4*/
        0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf, /*5*/
        0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8, /*6*/
        0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2, /*7*/
        0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73, /*8*/
        0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb, /*9*/
        0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79, /*a*/
        0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08, /*b*/
        0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a, /*c*/
        0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e, /*d*/
        0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf, /*e*/
        0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16  /*f*/
    };
    unsigned char invsBox[256] =  //逆置换表
    { /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f  */
        0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb, /*0*/
        0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb, /*1*/
        0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e, /*2*/
        0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25, /*3*/
        0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92, /*4*/
        0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84, /*5*/
        0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06, /*6*/
        0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b, /*7*/
        0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73, /*8*/
        0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e, /*9*/
        0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b, /*a*/
        0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4, /*b*/
        0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f, /*c*/
        0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef, /*d*/
        0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61, /*e*/
        0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d  /*f*/
    };
    //void *memcpy(void *destin, void *source, unsigned n)；
	//从源source中拷贝n个字节到目标destin中
	//返回值指向destin的指针
	//<cstring>/<string.h>
    memcpy(Sbox, sBox, 256);  //初始化S盒
    memcpy(InvSbox, invsBox, 256);

    KeyExpansion(key, w);
}

AES::~AES() //析构函数
{

}


//AES算法加密过程
unsigned char* AES::Cipher(unsigned char* input)
{
    unsigned char state[4][4];
    int i,r,c;

    //将输入的明文按序列组合成4*4矩阵
    for(r=0; r<4; r++)
    {
        for(c=0; c<4 ;c++)
        {
            state[r][c] = input[c*4+r];
        }
    }
    //与输入的密钥相加（异或），作为轮加密的输入
    AddRoundKey(state,w[0]);

    //循环10次
    for(i=1; i<=10; i++)
    {
        SubBytes(state);
        ShiftRows(state);
        if(i!=10) MixColumns(state);//最后一轮不进行列混淆变换
        AddRoundKey(state,w[i]);
    }

    //恢复序列
    for(r=0; r<4; r++)
    {
        for(c=0; c<4 ;c++)
        {
            input[c*4+r] = state[r][c];
        }
    }


    return input;
}

//解密过程
unsigned char* AES::InvCipher(unsigned char* input)
{
    unsigned char state[4][4];
    int i,r,c;
    //将输入的明文按序列组合成4*4矩阵
    for(r=0; r<4; r++)
    {
        for(c=0; c<4 ;c++)
        {
            state[r][c] = input[c*4+r];
        }
    }

    AddRoundKey(state, w[10]);
    for(i=9; i>=0; i--)
    {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, w[i]);
        if(i)
        {
            InvMixColumns(state);
        }
    }

    for(r=0; r<4; r++)
    {
        for(c=0; c<4 ;c++)
        {
            input[c*4+r] = state[r][c];
        }
    }

    return input;
}

//对外部数据的加密（处理字符串等）
//参数length为要加密的数据长度，默认值为0，表示是字符串
//加密时传进的指针要预留够16整数倍字节的空间，因为加密操作直接修改原数据，不足128位可能造成内存溢出
void* AES::Cipher(void* input, int length)
{
    unsigned char* in = (unsigned char*) input;
    int i;
    if(!length)    // 如果是默认值0则当做字符串处理
    {
        while(*(in+length++));
        in = (unsigned char*) input;
    }
    for(i=0; i<length; i+=16)
    {
        Cipher(in+i); //逐个加密
    }
    return input;
}

//对外部数据的解密
void* AES::InvCipher(void* input, int length)
{
    unsigned char* in = (unsigned char*) input;
    int i;
    for(i=0; i<length; i+=16)
    {
        InvCipher(in+i);
    }
    return input;
}

//密钥扩展
//将输入的密钥扩展为11组128位密钥组
void AES::KeyExpansion(unsigned char* key, unsigned char w[][4][4])
{
    int i,j,r,c;
    //轮常数 AES-128需要10轮
    unsigned char rc[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
    //第0组密钥为输入密钥本身
    for(r=0; r<4; r++)
    {
        for(c=0; c<4; c++)
        {
            w[0][r][c] = key[r+c*4];
        }
    }

    //后10组密钥
    for(i=1; i<=10; i++)
    {
        //第i组第j列为 第i-1组第j列 与 第i组第j-1列的模2和（1<=j<=3)
        for(j=0; j<4; j++)
        {
            unsigned char t[4];
            for(r=0; r<4; r++)
            {
                t[r] = j ? w[i][r][j-1] : w[i-1][r][3];
            }
            //对每一组密钥的第1列特殊处理
            if(j == 0)
            {
                unsigned char temp = t[0];
                //将第i-1组第3列的4个字节循环左移1个字节,并对每个字节进行字节替代变换
                for(r=0; r<3; r++)
                {
                    t[r] = Sbox[t[(r+1)%4]];
                }
                t[3] = Sbox[temp];
                t[0] ^= rc[i-1];//将第一行（即第一个字节）与轮常数相加
            }
            for(r=0; r<4; r++)
            {
                w[i][r][j] = w[i-1][r][j] ^ t[r];
            }
        }
    }
}


//字节替代
void AES::SubBytes(unsigned char state[][4])
{
    int r,c;
    for(r=0; r<4; r++)
    {
        for(c=0; c<4; c++)
        {
            state[r][c] = Sbox[state[r][c]];
        }
    }
}

//行移位变换
void AES::ShiftRows(unsigned char state[][4])
{
    unsigned char t[4];
    int r,c;
    for(r=1; r<4; r++) //对第2,3,4行移位变换，第一行不变
    {
        for(c=0; c<4; c++)
        {
            t[c] = state[r][(c+r)%4];
        }
        for(c=0; c<4; c++)
        {
            state[r][c] = t[c];
        }
    }
}

//void AES::ModifiedShiftRows(unsigned char state[][4])
//{
//    unsigned char t[4];
//    int r,c,rv1,rv2,rv3,rv4;
//
//    rv1 = (state[0][0]^k[0][0])^(state[0][1]^k[0][1])^(state[0][2]^k[0][2])^(state[0][3]^k[0][3]);
//    rv2 = (state[1][0]^k[1][0])^(state[1][1]^k[1][1])^(state[1][2]^k[1][2])^(state[1][3]^k[1][3]);
//    rv3 = (state[2][0]^k[2][0])^(state[2][1]^k[2][1])^(state[2][2]^k[2][2])^(state[2][3]^k[2][3]);
//    rv4 = (state[3][0]^k[3][0])^(state[3][1]^k[3][1])^(state[3][2]^k[3][2])^(state[3][3]^k[3][3]);
//
//    //求各行偏移量
//
//    for (int i = 0; i < 4; i++) {
//        for (int j = 0; j < 4 - i - 1; j++) {
//            if (arr[j] > arr[j + 1]) {
//                int temp = arr[j];
//                arr[j] = arr[j + 1];
//                arr[j + 1] = temp;
//            }
//        }
//    }
//
//
//
//
//    for(r=1; r<4; r++) //对第2,3,4行移位变换，第一行不变
//    {
//        for(c=0; c<4; c++)
//        {
//            t[c] = state[r][(c+r)%4];
//        }
//        for(c=0; c<4; c++)
//        {
//            state[r][c] = t[c];
//        }
//    }
//}


//列混淆变换
void AES::MixColumns(unsigned char state[][4])
{
    unsigned char t[4];
    int r,c;
    for(c=0; c< 4; c++)
    {
        for(r=0; r<4; r++)
        {
            t[r] = state[r][c];
        }
        for(r=0; r<4; r++)
        {
            state[r][c] = FFmul(0x02, t[r])
                        ^ FFmul(0x03, t[(r+1)%4])
                        ^ FFmul(0x01, t[(r+2)%4])
                        ^ FFmul(0x01, t[(r+3)%4]);
        }
    }
}

//轮密钥加变换
void AES::AddRoundKey(unsigned char state[][4], unsigned char k[][4])
{
    int r,c;
    for(c=0; c<4; c++)
    {
        for(r=0; r<4; r++)
        {
            state[r][c] ^= k[r][c];
        }
    }
}

//解密的基本运算   轮密钥加不变  其他都进行逆变换
//逆字节替代
void AES::InvSubBytes(unsigned char state[][4])
{
    int r,c;
    for(r=0; r<4; r++)
    {
        for(c=0; c<4; c++)
        {
            state[r][c] = InvSbox[state[r][c]];
        }
    }
}
//逆行移位
void AES::InvShiftRows(unsigned char state[][4])
{
    unsigned char t[4];
    int r,c;
    for(r=1; r<4; r++)
    {
        for(c=0; c<4; c++)
        {
            t[c] = state[r][(c-r+4)%4];
        }
        for(c=0; c<4; c++)
        {
            state[r][c] = t[c];
        }
    }
}
//逆列混合
void AES::InvMixColumns(unsigned char state[][4])
{
    unsigned char t[4];
    int r,c;
    for(c=0; c< 4; c++)
    {
        for(r=0; r<4; r++)
        {
            t[r] = state[r][c];
        }
        for(r=0; r<4; r++)
        {
            state[r][c] = FFmul(0x0e, t[r])
                        ^ FFmul(0x0b, t[(r+1)%4])
                        ^ FFmul(0x0d, t[(r+2)%4])
                        ^ FFmul(0x09, t[(r+3)%4]);
        }
    }
}


//有限域GF(2^8）上的乘法
//标准算法应该是循环8次（b与a的每一位相乘，结果相加），
//但这里只用到最低2位，解密时用到的逆列混淆也只用了低4位
//所以在这里高4位的运算是多余的，只计算低4位。
unsigned char AES::FFmul(unsigned char a, unsigned char b)
{
    unsigned char bw[4];
    unsigned char res=0;
    int i;
    bw[0] = b;
    for(i=1; i<4; i++)
    {
        bw[i] = bw[i-1]<<1;
        if(bw[i-1]&0x80)
        {
            bw[i]^=0x1b;
        }
    }
    for(i=0; i<4; i++)
    {
        if((a>>i)&0x01)
        {
            res ^= bw[i];
        }
    }
    return res;
}

/****************************/
////CBC模式
//void unitCipher(unsigned char list[16],unsigned char*iv)
//{
//    int i,j,k;
//    int num = 0;
//    int loopNum = 9;
//    unsigned char array[4][4];
//
//    for(i=0;i<4;i++)
//    {
//        for(j=0;j<4;j++)
//        {
//            array[j][i] = list[num++];
//            array[j][i]^= iv[num-1];
//        }
//    }
//
//    //与输入的密钥相加（异或），作为轮加密的输入
//    AddRoundKey(array,0);
//
//    //循环
//    for(i=0; i<loopNum; i++)
//	{
//		for(j=0;j<4;j++)
//		{
//			for(k=0;k<4;k++)
//			{
//				array[k][j] = SboxSubt(array[k][j], Sbox);
//			}
//
//		}
//
//		shiftRows(array);
//		mixColumns(array);
//		AddRoundKey(array, (i+1)*4);
//	}//loopNum
//
//
//    for(j=0;j<4;j++)
//	{
//		for(k=0;k<4;k++)
//		{
//			array[k][j] = SboxSubt(array[k][j], Sbox);
//		}
//
//	}
//	//the last time shift rows
//	shiftRows(array);
//	//the last time key encrypt
//	AddRoundKey(array, 40);
//
//	num = 0;
//	for(i=0;i<4;i++)
//	{
//		for(j=0;j<4;j++)
//		{
//			list[num++] = array[j][i];
//		}
//	}
//	if(enCount%1000==0)
//		printf("The %-5d encryption process complted!\n", enCount);
//	enCount++;
//}
//

//
//unsigned char* AES::CBC_Cipher(unsigned char* input)
//{
//    unsigned char state[4][4];
//    int i,r,c;
//
//    //unsigned char iv[16] = {0};
//    memcpy(iv,IV,16);//IV是设置的初始化向量
//
//
//    //将输入的明文按序列组合成4*4矩阵
//    for(r=0; r<4; r++)
//    {
//        for(c=0; c<4 ;c++)
//        {
//            state[r][c] = input[c*4+r];
//        }
//        //明文分组第一组与iv异或
//        unitCipher(state[r][c],iv);
//        iv = state[r][c];
//    }
//
//
//
//    //恢复序列
//    for(r=0; r<4; r++)
//    {
//        for(c=0; c<4 ;c++)
//        {
//            input[c*4+r] = state[r][c];
//        }
//    }
//
//    return input;
//}

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#include<iostream>
#include<string>


#define LHB(N) ((N) & 15)	//lower half byte
#define HHB(N) ((N) >> 4 & 15)	//higher half byte


const char *trailer_description[8] = 
{
	"never     key A     key A     never     key A     key A",
	"never     key A     key A     key A     key A     key A",
	"never     never     key A     never     key A     never",
	"never     key B     key A|B   key B     never     key B",
	"never     key B     key A|B   never     never     key B",
	"never     never     key A|B   key B     never     never",
	"never     never     key A|B   never     never     never",
	"never     never     key A|B   never     never     never"
};

const char *normal_description[8] = 
{
	"key A|B   key A|B   key A|B   key A|B",
	"key A|B   never     never     key A|B",
	"key A|B   never     never     never",
	"key B     key B     never     never",
	"key A|B   key B     never     never",
	"key B     never     never     never",
	"key A|B   key B     key B     key A|B",
	"never     never     never     never"
};

int main()
{
	while (1)
	{
		std::string line;
		printf("input byte 6, 7, 8 of block 3 (hexadecimal format): ");
		if (std::getline(std::cin, line).eof()) break;
		
		uint8_t data[3];
		if(sscanf(line.c_str(), "%X %X %X", &data[0], &data[1], &data[2]) != 3)
			continue;
		
		//check complement
		if (LHB(data[0]) != HHB(~data[1])
		 || HHB(data[0]) != LHB(~data[2])
		 || LHB(data[1]) != HHB(~data[2])
		)
		{
			printf("not valid sequence!\n");
			continue;
		}
		
		printf("\n                read      write     increment dec/tra/res\n");
		for(int i = 0; i < 4; i++)
		{
			int acb = 0;
			
			acb |= (data[1] >> (4+i)) & 1;
			acb <<= 1;
			acb |= (data[2] >>   i  ) & 1;
			acb <<= 1;
			acb |= (data[2] >> (4+i)) & 1;
			
			if(i == 3)
			{
				printf("\n                KEYA                Access Bits         KEYB\n");
				printf("                read      write     read      write     read      write\n");
				printf("Access Bit[%d]:  %s\n\n", i, trailer_description[acb]);
			}
			else
			{
				printf("Access Bit[%d]:  %s\n", i, normal_description[acb]);
			}
		}
	}
	
	return 0;
}

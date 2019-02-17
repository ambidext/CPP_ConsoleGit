// Calculator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI       3.14159265358979323846   // pi

void IntCalc(char * out, char * input)
{
	char numStack[256][256] = { { 0, }, };
	int nsIdx = 0;
	char strNum[256] = { 0, };
	int snIdx = 0;

	// 숫자 추출 & factorial 처리 
	for (int i = 0; i < strlen(input); i++)
	{
		if (input[i] >= '0' && input[i] <= '9')
		{
			strNum[snIdx++] = input[i];
		}
		else // +-*/ 기호 
		{
			if (snIdx > 0)
				sprintf(numStack[nsIdx++], "%s", strNum);
			memset(strNum, 0, sizeof(strNum)); snIdx = 0;
			sprintf(numStack[nsIdx++], "%c", input[i]);
		}
	}
	if (snIdx > 0)
	{
		sprintf(numStack[nsIdx++], "%s", strNum);
	}

	// 곱하기, 나누기 처리 
	char calcStack[256][256] = { { 0, }, };
	int csIdx = 0;

	for (int i = 0; i<nsIdx; i++)
	{
		if (strcmp(numStack[i], "*") == 0 || strcmp(numStack[i], "/") == 0 || strcmp(numStack[i], "+") == 0 || strcmp(numStack[i], "-") == 0)
		{
			sprintf(calcStack[csIdx++], "%s", numStack[i]);
		}
		else
		{
			if (csIdx > 0 && strcmp(calcStack[csIdx - 1], "*") == 0)
			{
				int num2 = atoi(numStack[i]);
				--csIdx;
				char * str = calcStack[--csIdx];
				int num1 = atoi(str);

				sprintf(calcStack[csIdx++], "%d", num1*num2);
			}
			else if (csIdx > 0 && strcmp(calcStack[csIdx - 1], "/") == 0)
			{
				int num1 = atoi(numStack[i]);
				--csIdx;
				char * str = calcStack[--csIdx];
				int num2 = atoi(str);
				if (num2 == 0)
				{
					sprintf(out, "INF");
					return;
				}

				sprintf(calcStack[csIdx++], "%d", num2 / num1);
			}
			else
			{
				sprintf(calcStack[csIdx++], "%s", numStack[i]);
			}
		}
	}

	//// 더하기, 빼기 처리 

	int total = 0;
	int i = 0;
	if (strcmp(calcStack[0], "+") == 0 || strcmp(calcStack[0], "-") == 0) // '+/-'기호가 먼저 나올 경우 처리
	{
		total = 0; 
	}
	else
	{
		total = atoi(calcStack[i++]);
	}

	while (i < csIdx)
	{
		char * sym = calcStack[i++];
		int num = atoi(calcStack[i++]);

		if (sym[0] == '+')
			total += num;
		else if (sym[0] == '-')
			total -= num;
	}

	sprintf(out, "%d", total);
}

void NormalCalc(char * result, char * input)
{
	//-------------------------------
	// 괄호 처리 
	char charStack[256] = { 0, };
	int csIdx = 0;
	char inBrace[256] = { 0, };
	char ibIdx = 0;

	for (int i = 0; i<strlen(input); i++)
	{
		if (input[i] == ')')
		{
			while (true)
			{
				char temp = charStack[--csIdx];
				if (temp == '(')
				{
					char dc[256];
					char reverse[256] = { 0, };
					for (int c = 0; c < ibIdx; c++)
					{
						reverse[c] = inBrace[ibIdx - 1 - c];
					}
					IntCalc(dc, reverse);
					for (int i = 0; i<strlen(dc); i++)
					{
						charStack[csIdx++] = dc[i];
					}

					memset(inBrace, 0, sizeof(inBrace)); ibIdx = 0;
					break;
				}
				else
				{
					inBrace[ibIdx++] = temp;
				}
			}
		}
		else
		{
			charStack[csIdx++] = input[i];
		}
	}

	charStack[csIdx] = 0;
	IntCalc(result, charStack);
	//-------------------------------
}


void Conv10(char * out, char * strNum)
{
	int len = strlen(strNum);
	int decNum = 0;

	if ((len > 1) && (strNum[0] == '0' && strNum[1] == 'x'))
	{
		char * hexNum = (strNum+2);
		for (int i = 0; i<strlen(hexNum); i++)
		{
			decNum += (hexNum[strlen(hexNum) - 1 - i] - '0') * (int)pow(16.0, (double)i);
		}
		sprintf(out, "%d", decNum);
		return;
	}
	else if ((len > 1) && strNum[len - 1] == 'b')
	{
		for (int i = 0; i < len - 1; i++)
		{
			decNum += (strNum[len - 2 - i] - '0') * (int)pow(2.0, (double)i);
		}
		sprintf(out, "%d", decNum);
		return;
	}
	else
	{
		sprintf(out, "%s", strNum);
		return;
	}
}

void ConvBin(char * out, int num)
{
	int mask;
	int idx = 0;
	int firstIdx = 0;
	unsigned char intNum[32] = { 0, };
	for (int i = 0; i < 32; i++)
	{
		mask = 1 << i;
		if (num&mask)
		{
			intNum[31 - i] = 1;
			firstIdx = 31 - i;
		}
		else
		{
			intNum[31 - i] = 0;
		}
	}

	for (int i = firstIdx; i < 32; i++)
	{
		out[i - firstIdx] = intNum[i] + '0';
	}
}

void ProgrammerCalc(char * result, char * input)
{
	//-------------------------------
	char strTotal[256] = { 0, };
	int tIdx = 0;
	char strNum[256] = { 0, }; 
	int sIdx = 0;

	// 숫자 추출 & 10진수 변환
	for (int i = 0; i < strlen(input); i++)
	{
		if ((input[i] >= '0' && input[i] <= '9') || (input[i] == 'x') || input[i] == 'b')
		{
			strNum[sIdx++] = input[i];
		}
		else
		{
			if (sIdx > 0)
			{
				char out[256] = { 0, };
				Conv10(out, strNum);
				strcat(strTotal, out);
				tIdx += strlen(out);
			}
			memset(strNum, 0, sizeof(strNum));
			sIdx = 0; 
			strTotal[tIdx++] = input[i]; 
		}
	}
	if (sIdx > 0)
	{
		char out[256] = { 0, };
		Conv10(out, strNum);
		strcat(strTotal, out);
	}

	NormalCalc(result, strTotal);
	int resNum = atoi(result);
	char bin[256] = { 0, };
	ConvBin(bin, resNum);
	sprintf(result, "%d 0x%X %sb", resNum, resNum, bin);
	//-------------------------------
}

char *replaceAll(char *s, const char *olds, const char *news) {
	char *result, *sr;
	size_t i, count = 0;
	size_t oldlen = strlen(olds); if (oldlen < 1) return s;
	size_t newlen = strlen(news);


	if (newlen != oldlen) {
		for (i = 0; s[i] != '\0';) {
			if (memcmp(&s[i], olds, oldlen) == 0) count++, i += oldlen;
			else i++;
		}
	}
	else i = strlen(s);

	result = (char *)malloc(i + 1 + count * (oldlen > newlen ? oldlen-newlen : newlen-oldlen));
	if (result == NULL) return NULL;

	sr = result;
	while (*s) {
		if (memcmp(s, olds, oldlen) == 0) {
			memcpy(sr, news, newlen);
			sr += newlen;
			s += oldlen;
		}
		else *sr++ = *s++;
	}
	*sr = '\0';

	return result;
}

int factorial(int n)
{
	if (n == 0 || n == 1)
		return 1;
	return n * factorial(n - 1);
}

double GetTriValue(char * strNum)
{
	double dNum;
	if (strNum[0] == 'S')
	{
		dNum = atof(&strNum[1]);
		dNum = sin(dNum*PI/180.0);
	}
	else if (strNum[0] == 'C')
	{
		dNum = atof(&strNum[1]);
		dNum = cos(dNum * PI / 180.0);
	}
	else if (strNum[0] == 'T')
	{
		dNum = atof(&strNum[1]);
		dNum = tan(dNum * PI / 180.0);
	}
	else if (strNum[0] == 'L')
	{
		dNum = atof(&strNum[1]);
		dNum = log10(dNum);
	}
	else
	{
		dNum = atof(strNum);
	}
	return dNum;
}

void DoubleCalc(char * out, char * input)
{
	char numStack[256][256] = { { 0, }, };
	int nsIdx = 0;
	char strNum[256] = { 0, };
	int snIdx = 0;

	// 숫자 추출 & factorial 처리 
	for (int i = 0; i < strlen(input); i++)
	{
		if (input[i] >= '0' && input[i] <= '9' || input[i] == '.' || input[i] == 'S' || input[i] == 'C' || input[i] == 'T' || input[i] == 'L')
		{
			strNum[snIdx++] = input[i];
		}
		else if (input[i] == '!')
		{
			int nNum = factorial(atoi(strNum));
			memset(strNum, 0, sizeof(strNum)); snIdx = 0;
			sprintf(numStack[nsIdx++], "%d", nNum); 
		}
		else // +-*/ 기호 
		{
			double dNum = GetTriValue(strNum);
			sprintf(numStack[nsIdx++], "%lf", dNum); 
			memset(strNum, 0, sizeof(strNum)); snIdx = 0;
			sprintf(numStack[nsIdx++], "%c", input[i]); 
		}
	}
	if (snIdx > 0)
	{
		double dNum = GetTriValue(strNum);
		sprintf(numStack[nsIdx++], "%lf", dNum);
	}

	// 곱하기, 나누기, 거듭제곱 처리 
	char calcStack[256][256] = { { 0, }, };
	int csIdx = 0;

	for(int i=0; i<nsIdx; i++)
	{
		if (strcmp(numStack[i],"*") == 0 || strcmp(numStack[i], "/")==0 || strcmp(numStack[i], "+")==0 || strcmp(numStack[i], "-")==0 || strcmp(numStack[i], "^") == 0)
		{
			sprintf(calcStack[csIdx++], "%s", numStack[i]); 
		}
		else
		{
			if (csIdx > 0 && strcmp(calcStack[csIdx-1],"*") == 0)
			{
				double num2 = atof(numStack[i]);
				--csIdx; 
				char * str = calcStack[--csIdx]; 
				double num1 = atof(str); 

				sprintf(calcStack[csIdx++], "%lf", num1*num2); 
			}
			else if (csIdx > 0 && strcmp(calcStack[csIdx - 1], "/") == 0)
			{
				double num1 = atof(numStack[i]);
				--csIdx;
				char * str = calcStack[--csIdx]; 
				double num2 = atof(str);
				if (num2 == 0)
				{
					sprintf(out, "INF");
					return;
				}

				sprintf(calcStack[csIdx++], "%lf", num2 / num1); 
			}
			else if (csIdx > 0 && strcmp(calcStack[csIdx - 1], "^") == 0)
			{
				double num1 = atof(numStack[i]);
				--csIdx;
				char * str = calcStack[--csIdx]; 
				double num2 = atof(str);

				sprintf(calcStack[csIdx++], "%lf", pow(num2, num1));
			}
			else
			{
				sprintf(calcStack[csIdx++], "%s", numStack[i]); 
			}
		}
	}

	//// 더하기, 빼기 처리 

	if (strcmp(calcStack[csIdx-1],"+")==0 || strcmp(calcStack[csIdx-1], "-") == 0) // '+/-'기호가 먼저 나올 경우 처리
		sprintf(calcStack[csIdx++], "0");

	int i = 0; 
	double total = atof(calcStack[i++]);
	while (i < csIdx)
	{
		char * sym = calcStack[i++];
		double num = atof(calcStack[i++]);

		if (sym[0] == '+')
			total += num;
		else if (sym[0] == '-')
			total -= num; 
	}

	sprintf(out, "%lf", total);
}

void EngineeringCalc(char * result, char * input)
{
	//------------------------------
	input = replaceAll(input, "SIN", "S");
	input = replaceAll(input, "COS", "C");
	input = replaceAll(input, "TAN", "T");
	input = replaceAll(input, "LOG", "L");

	// 괄호 처리 
	char charStack[256] = { 0, };
	int csIdx = 0;
	char inBrace[256] = { 0, };
	char ibIdx = 0;

	for (int i = 0; i<strlen(input); i++)
	{
		if (input[i] == ')')
		{
			while (true)
			{
				char temp = charStack[--csIdx]; 
				if (temp == '(')
				{
					char dc[256];
					char reverse[256] = { 0, };
					for (int c = 0; c < ibIdx; c++)
					{
						reverse[c] = inBrace[ibIdx - 1 - c];
					}
					DoubleCalc(dc, reverse);
					for(int i=0; i<strlen(dc); i++)
					{
						charStack[csIdx++] = dc[i]; 
					}

					memset(inBrace, 0, sizeof(inBrace)); ibIdx = 0;
					break;
				}
				else
				{
					inBrace[ibIdx++] = temp;
				}
			}
		}
		else
		{
			charStack[csIdx++] = input[i];
		}
	}

	DoubleCalc(result, charStack);
	double dOut = atof(result);
	sprintf(result, "%.3lf", dOut);
	//------------------------------
}

int main()
{
	char result[256];
	char NormalInput[] = "12+3*(7+3*2)+19/(3*(3/2))";// "-2+5*7+19/((2+1)*(2+1))";//
	char ProgrammerInput[] = "10b+0x23+9/3";//"11b+(0x11*10b+1)-9";//
	char EngineeringInput[] = "-2+2^(1/2)+SIN((40+5)*2)+LOG(2*3+4)+5!";//"-0.123+2^(1/2)+SIN((40+5)*SIN(89+SIN(90)))+LOG(2*3+4)+5!"; // 

	memset(result, 0, sizeof(result));
	printf("Normal Calculator: %s\n", NormalInput);
	NormalCalc(result, NormalInput);
	printf("%s\n", result);
	printf("\n"); 

	memset(result, 0, sizeof(result));
	printf("Programmer Calculator: %s\n", ProgrammerInput);
	ProgrammerCalc(result, ProgrammerInput);
	printf("%s\n", result);
	printf("\n");

	memset(result, 0, sizeof(result));
	printf("Engineering Calculator: %s\n", EngineeringInput);
	EngineeringCalc(result, EngineeringInput);
	printf("%s\n", result);
	printf("\n");

    return 0;
}


#include "func.h"
#include <stdio.h>
#include <string.h>

namespace func {

	void func_main() {
		int a1 = 10, a2 = 20, a3 = 30,a4 = 30,a5 = 30;
		printf("%d \t%d \t%d \t%d \t%d \t \n", a1, a2, a3, a4, a5);
		func_int(a1, a2, &a3, &a4, &a5);
		printf("%d \t%d \t%d \t%d \t%d \t \n", a1, a2, a3, a4, a5);

		auto fcc1 = new funcc(110);
		auto fcc2 = new funcc(120);

		funcc* fcc3;
		funcc* fcc4;
		fcc3= new funcc(130);
		fcc4= new funcc(140);

		fcc1->PlusPub(3);
		fcc2->PlusPub(3);
		fcc1->PlusPubC(5);
		fcc2->PlusPubC(5);

		delete fcc1;
		delete fcc2;
		delete fcc3;
		delete fcc4;
		printf("*****END*******");
	}

	/// <summary>
	/// ����Ĭ��ֵ�Ĳ��� ����
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <returns></returns>
	int func_params(int a, int b, int c) {
		if (b < 0) {
			b = b * (-1) * 100;
		}
		if (c < 0) {
			c = c * (-1) * 1000;
		}
		return a + b + c;
	}

	/// <summary>
	/// const ���������� ��ָ�� int
	/// </summary>
	/// <param name="a"></param>
	/// <param name="ca"></param>
	/// <param name="pa"></param>
	/// <param name="pca"></param>
	/// <param name="ppca"></param>
	void func_int(int a, const int ca, int* pa, const int* pca, int* const ppca) {
		a = 100;

		//ca = 300; //ֻ������ ����ֱ���޸�
		int* pb = (int*)&ca; //ֻ������ ����ͨ��ָ���޸�
		*pb = 201;

		*pa = 300;

		//*pca = 400; //ֻ����������ֱ���޸�
		int* pd = (int*)pca;//ֻ������ ����ͨ��ָ���޸�
		*pd = 401;

		*ppca = 500;
	}

	/// <summary>
	/// const ���������� ��ָ�� char
	/// </summary>
	/// <param name="a"></param>
	/// <param name="ca"></param>
	/// <param name="pa"></param>
	/// <param name="pca"></param>
	/// <param name="cpa"></param>
	void func_char(char a[],  const char ca[], char* pa, const char* pca, char* const cpa) {
		int len = strlen(a);

	}


	int funcc::SNumPub = 1000;
	int funcc::SNumPri = 2000;
	int funcc::SNumPro = 3000;

	funcc::funcc()
	{
		this->ID = -1;
		this->NumPub = 100;
		this->NumPri = 200;
		this->NumPro = 300;
		printf("Construtor NULL\n");
	}
	funcc::funcc(int id)
		:func::funcc()
	{
		ID = id;
		printf("Construtor %d \n", id);
		/*this->NumPub = 100;
		this->NumPri = 200;
		this->NumPro = 300;*/
	}

	funcc::~funcc()
	{
		printf("disposed ID:\t %d \n", ID);
	}

	void funcc::PlusPub(int x)
	{
		this->NumPub+=x;
		this->NumPri+=x;
		this->NumPro+=x;
		printf("PlusPub\t NumPub \t%d\n", this->NumPub);
		printf("PlusPub\t NumPri \t%d\n", this->NumPri);
		printf("PlusPub\t NumPro \t%d\n", this->NumPro);

		funcc::SNumPub+=x;
		funcc::SNumPri+=x;
		funcc::SNumPro+=x;
		printf("PlusPub\t SNumPub \t%d\n", funcc::SNumPub);
		printf("PlusPub\t SNumPri \t%d\n", funcc::SNumPri);
		printf("PlusPub\t SNumPro \t%d\n", funcc::SNumPro);

		printf("PlusPub\t CNumPub \t%d\n", this->CNumPub);
		printf("PlusPub\t CNumPri \t%d\n", this->CNumPri);
		printf("PlusPub\t CNumPro \t%d\n", this->CNumPro);
		printf("**********************\n", this->CNumPro);

	}
	void funcc::PlusPubC(int x) const
	{
		printf("PlusPubC CNumPub:\t%d\n", this->CNumPub);
		int* pc = (int*)(&this->CNumPub);
		*pc = *pc + x;
		printf("PlusPubC CNumPub:\t%d\n", this->CNumPub);
		printf("**********************\n", this->CNumPro);
	}

	void funcc::PlusPri(int x)
	{
	}
	void funcc::PlusPriC(int x) const
	{
	}
	void funcc::PlusPro(int x)
	{
	}
	void funcc::PlusProC(int x)
	{
	}
}
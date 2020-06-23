#pragma once

namespace func {
	class funcc
	{
	private:
		int ID;

	public:
		int NumPub;
		static int SNumPub;
		const int CNumPub = 10000;
	private:
		int NumPri;
		static int SNumPri;
		const int CNumPri = 20000;
	protected:
		int NumPro;
		static int SNumPro;
		const int CNumPro = 30000;

	public:
		void PlusPub(int x);
		void PlusPubC(int y) const;
		funcc();
		funcc(int id);
		~funcc();
	private:
		void PlusPri(int x);
		void PlusPriC(int x) const;
	protected:
		void PlusPro(int x);
		void PlusProC(int x);
	};

	void func_main();

	int func_params(int a, int b = -1, int c = -2);

	void func_int(int a, const int ca, int* pa, const int* pca, int* const ppca);

	void func_char(char a[], const char ca[], char* pa, const char* pca, char* const cpa);
}

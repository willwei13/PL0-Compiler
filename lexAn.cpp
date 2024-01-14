
#include"recursion.h"
using namespace std;

int main()
{
	lex("test.txt");
	for (int i = 0; i < 100; i++)
		cout << result[i] << " ";
	cout << endl;
	for (int i = 0; i < 100; i++)
		cout << slabel[i] << " ";
	cout << endl;
	for (int i = 0; i < 100; i++)
		cout << clabel[i] << " ";
	cout << endl;
	program();
	cout << endl;
	print(Tprogram,0);
	
	cout << "\t index"<<" \t name: " << "\t kind: "  << "\t level/value: "<< "\t addr: "  << endl;
	for (int i = 0; i <tableId; i++)
		cout <<" \t "<<i<<" \t "<< table[i].name << " \t " << table[i].kind << " \t " 
		<< table[i].attribute << " \t\t " << table[i].addr << endl;
	cout << endl;
	
	cout << "\t index" << " \t f: " << "\t l: " << "\t a: " <<endl;
	for (int i = 0; i <codeId; i++) 
		cout << " \t " << i << " \t " << code[i].funcCode << " \t " << code[i].levelDiff << " \t "
		<< code[i].displacement << "\t"<<code[i].content<<endl;
	explainToPerfrom();
	system("pause");
	return 0;
}
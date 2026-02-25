#include <iostream>
using namespace std;

int main()
{
  int n;
  cout << "Veuillez saisir un nombre n" << endl;
  cin >> n;
  if (n>5)
	cout << "Le nombre que vous avez saisi est superieur a 5" << endl;
  else
	cout << "Vous avez saisi un nombre inferieur a 5, Reessayer !" << endl;

  return 0;
}

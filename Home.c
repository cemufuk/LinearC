#include <stdio.h>
#include "Main.h"

Menu m_home = {
	// Başlık
	"LineerC | Ana Menu",
	// Seçenekler
	{
		"Cikis",
		"Matris Tanimla"
	},
	// Seçeneklerin çalıştıracağı fonksiyonlar
	{
		mf_back,
		mf_define
	}
};

int mf_define(MxMemory* memory)
{
	printf("Henuz tanimlanmadi.");
	(void)getchar();
	return 1;
}
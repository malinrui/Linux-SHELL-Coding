#include <stdio.h>
#include "ls_header.h"
 
int main(int argc, char **argv) {
	// 处理参数, lsLong是带有 -l 参数的, lsShort为没有-l参数的ls命令
	int i;
	if (argc == 1) {						/* ls . */
		lsShort("./");				
		return 0;
	}
	if (argc == 2 && !strcmp(argv[1], "-l") ) {			/* ls -l . */
		lsLong("./");
		return 0;
	}
	if (argc > 2 && !strcmp(argv[1], "-l") ) {			/* ls -l dirname */
		for(i = 2; i < argc; ++ i) {
			printf("%s:\n", argv[i]);
			lsLong(argv[i]);
			if(i != argc - 1)
				printf("\n");	
		}
		return 0;	
	} else {							/* ls dirname */
		for (i = 1; i < argc; ++ i) {
			printf("%s:\n", argv[i]);
			lsShort(argv[i]);
			if(i != argc - 1)
				printf("\n");	
		}
		return 0;
	}
	
	return 0;
}

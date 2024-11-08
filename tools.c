#include <stdio.h>
#include <kami.h>
#include <notice.h>
#include <string.h>
int main() {
	notice("https://w.yandata.cn/api/fN59g96iubWs17Xi");
	char kami[100];
	printf("Enter Kami: ");
	fgets(kami, sizeof(kami), stdin); // 使用fgets代替scanf
	kami[strcspn(kami, "\n")] = 0; // 去除fgets读取的换行符
	if(kami_check("https://w.yandata.cn/api/3ehXWJv4p7KZDSIt", 525, kami) == 1) {
		printf("False\n");
		return 1;
	} else {
		printf("True\n");
		return 0;
	}
}

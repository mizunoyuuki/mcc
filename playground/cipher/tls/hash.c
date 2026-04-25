#include<stdlib.h>
#include<stdio.h>
#include<openssl/evp.h>

#define BUFFER_SIZE 4096

void handle_errors(){
	fprintf(stderr, "エラーが発生しました\n");
	exit(1);
}

int main(int argc, char *argv[]){
	if (argc != 2){
		fprintf(stderr, "usage: %s<ハッシュ計算したいファイル>\n", argv[0]);
		return 1;
	}

	const char *filename = argv[1];
	FILE *file = fopen(filename, "rb");

	if (!file){
		perror("ファイルを開けません\n");
		return 1;
	}

	// opensslのコンテキスト(状態保持用の構造体)を準備
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();

	if (mdctx == NULL) handle_errors();

	// sha-256アルゴリズムを使用するように初期化
	if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)){
		handle_errors();
	}

	unsigned char buffer[BUFFER_SIZE];
	size_t bytes_read;
	while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0){
		if (1 != EVP_DigestUpdate(mdctx, buffer, bytes_read)){
			handle_errors();
		}
	}

	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;
	if (1 != EVP_DigestFinal_ex(mdctx, md_value, &md_len)){
		handle_errors();
	}

	EVP_MD_CTX_free(mdctx);
	fclose(file);

	printf("SHA-256 Hash of '%s'\n", filename);
	for (unsigned int i = 0; i < md_len; i++){
		printf("%02x", md_value[i]);
	}

	printf("\n");

	return 0;
}


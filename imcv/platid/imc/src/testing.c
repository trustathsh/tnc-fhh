#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <tss/tss_defines.h>

const char engine_id[] = "tpm";

#define TPM_CMD_PIN	ENGINE_CMD_BASE + 1

int main()
{
	int r;
	ENGINE *e;

	/* Make the "dynamic" ENGINE available */
	ENGINE_load_dynamic();
	e = ENGINE_by_id(engine_id);

	if (!e) {
		fprintf(stderr, "Failure loading tpm engine!!!\n");
		exit(1);
	}

	fprintf(stderr, "TPM engine loaded!\n");

	/* returns zero on failure */
	r = ENGINE_init(e);
	if (!r) {
		fprintf(stderr, "Failure inititializing tpm engine!!!\n");
		exit(1);
	}

	fprintf(stderr, "TPM engine initialized!\n");

	/* setting the srk secret
	 * */
	ENGINE_ctrl(e, TPM_CMD_PIN, 1, NULL, NULL);

	r = ENGINE_set_default_RSA(e);

	if (!r) {
		fprintf(stderr, "Failure setting default RSA method\n");
	} else {
		fprintf(stderr, "Default RSA set to tpm engine\n");
		char signature[256] = {0};
		int i;

		EVP_PKEY *evpkey = ENGINE_load_private_key(e, "mykez", NULL, NULL);
		RSA *rsa1 = evpkey->pkey.rsa;
		
		r = RSA_private_encrypt(10, "ARNEWELZEL", signature, rsa1, RSA_PKCS1_PADDING);
		if (!r) {
			fprintf(stderr, "Error encrypting!\n");
			exit(1);
		}

		for (i = 0; i < 256; i++)
			fprintf(stderr, "%02hhX%s",
					signature[i],
					(((i+1) % 20) == 0) ? "\n" : " ");
		fprintf(stderr, "\n");

		FILE * mysig = fopen("mysig","w+");

		if(mysig) {
			fwrite(signature, 256, 1, mysig);
			fclose(mysig);
		} else {
			fprintf(stderr, "argghhh!!!");
		}



	}

	r = ENGINE_finish(e);
	if (!r) {
		fprintf(stderr, "Failure finishing tpm engine!!!\n");
		exit(1);
	}

	fprintf(stderr, "Finished up tpm engine!!!\n");

	ENGINE_free(e);

	ENGINE_cleanup();

	return 0;
}

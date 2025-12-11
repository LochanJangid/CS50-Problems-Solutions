#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{

    // Get Key
    char *key = argv[1];

    int length = strlen(key);

    // Validate Key
    for (int i = 0; i < length; i++)
    {
        if (!((key[i] >= 'A' && key[i] <= 'Z') || (key[i] >= 'a' && key[i] <= 'z')))
        {
            printf("bro you should type alphabets");
            return 1;
        }

        for (int j = 0; j < length; j++)
        {
            if (key[i] == key[j] && !(i == j))
            {
                printf("Duplicate bro!");
                return 1;
            }
        }
    }

    // Validate key length
    if (length != 26)
    {
        printf("Length should be 26");
        return 1;
    }

    // Get Plain Text
    char plainText[100];
    printf("Plain Text: ");
    fgets(plainText, sizeof plainText, stdin);

    // Make a alphabet table for mapping key

    char cipher[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char cipher2[] = "abcdefghijklmnopqrstuvwxyz";

    char cipherText[100]; // Ciper Text

    for (int i = 0; i < 26; i++)
    {
        if (plainText[i] >= 'A' && plainText[i] <= 'Z')
        {
            for (int j = 0; j < 26; j++)
            {
                if (plainText[i] == cipher[j])
                {
                    if (key[j] >= 'A' && key[j] <= 'Z')
                        cipherText[i] = key[j];
                    else
                        cipherText[i] = key[j] - 32;
                }
            }
        }
        else if (plainText[i] >= 'a' && plainText[i] <= 'z')
        {
            for (int j = 0; j < 26; j++)
            {
                if (plainText[i] == cipher2[j])
                {
                    if (key[j] >= 'a' && key[j] <= 'z')
                        cipherText[i] = key[j];
                    else
                        cipherText[i] = key[j] + 32;
                }
            }
        }
        else
            cipherText[i] = plainText[i];
    }
    printf("Cipher Text: %s\n", cipherText);
}
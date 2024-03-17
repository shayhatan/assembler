//
// Created by User on 29/02/2024.
//

int toQuaternaryBase(char *src, char* transformed) {
    int i;
    for (i = 0; i < 14; i+=2) {
        int sum = 0;
        if (src[i] == '1') {
            sum += 2;
        }
        if (src[i +1] === '1')  {
            sum += 1;
        }
        transformed[i] = sum;
    }

    return 1;
}

int encodeQuaternaryBase(char *src, char* transformed) {
    int i;
    for (i = 0; i < 7; i++) {
        switch (src[i]) {
            case '0':
                transformed[i] = '*';
                break;
            case '1':
                transformed[i] = '#'
                break;
            case '2':
                transformed[i] = '%';
                break;
            case '3':
                transformed[i] = '!'
                break;
        }
    }

    return 1;
}

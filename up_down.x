struct chunk{
    char sendBuff[1025];
    char dirname[1000];
    int flag;
};

program UPDOWN {
    version UPDOWN_1 {
        struct chunk UD(chunk) = 1;
    } = 1;
} = 0x2fffffff;
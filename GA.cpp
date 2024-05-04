/**
    chuoi NST co dang : L, U, R, D, X (khong lam gi) ; do dai T
    lai tao : cat doi, cat ba
    bien di : thay doi random 1 vi tri, co the cai tien len bien di nhieu vi tri
    cai tien :
    - lam cai ham kiem tra tu dong xem o empty co bi di ra ngoai khong
    - co the tan dung cac o X de lam cho o empty nam trong bang sau khi bien di/ lai tao
    - luu giu lai 80% NST tot nhat + 20% NST bat ky

    thay doi co chu dich xong check
*/
#include <bits/stdc++.h>

#define pis pair<int, string>

using namespace std;

using namespace chrono;

class timer: high_resolution_clock {
    const time_point start_time;
public:
    timer(): start_time(now()) {}
    rep elapsed_time() const { return duration_cast<milliseconds>(now()-start_time).count(); } };

const int Ti = 2900;
timer t; /// create a timer object

const int maxNST = 25;

int arr[10][10], brr[10][10], N, T; /// input
vector<string> NST;
int bdX, bdY;

char dir[] = {'L', 'U', 'R', 'D', 'X'};
int dx[] = {0, -1, 0, 1, 0};
int dy[] = {-1, 0, 1, 0, 0};
int idx[] = {0, 1, 2, 3, 4};

bool isKeNhau[16][16][4];

bool isBitOn(int i,int j){  /// whether bit j of i equal 1 ?
    return ((i >> j) & 1);
}

void init(){    /// chuan bi truoc cac o co the ke nhau
    for(int i = 1;i < 16;i ++){
        for(int j = 1;j < 16;j ++){
            for(int k = 0;k < 4;k ++){
                if(isBitOn(i, k) && isBitOn(j, (k + 2) % 4)){
                    isKeNhau[i][j][k] = true;
                }   /// o thu nhat co gia tri i, o ke dang xet co gia tri j
            }       /// o thu nhat sang o thu hai theo huong k
        }
    }
}

bool isInside(int x,int y){
    return 1 - (x < 0 || x >= N || y < 0 || y >= N);
}

int vis[10][10], ff;

int calScore(string nst){
    for(int i = 0;i < N;i ++){
        for(int j = 0;j < N;j ++){
            brr[i][j] = arr[i][j];
        }
    }

    int x = bdX, y = bdY;

    for(char i : nst){
        int huong;

        switch(i){
            case 'L' : {
                huong = 0;
                break;
            }
            case 'U' : {
                huong = 1;
                break;
            }
            case 'R' : {
                huong = 2;
                break;
            }
            case 'D' : {
                huong = 3;
                break;
            }
            default:{
                huong = 4;
                break;
            }
        }

        swap(brr[x][y], brr[x + dx[huong]][y + dy[huong]]);

        x += dx[huong], y += dy[huong];
    }

    ff ++;

    int ans = 0;
    queue<pair<int, pair<int, int> > > q;
    /// previous direction, (x, y)
    /// not go to previous square

    for(int i = 0;i < N;i ++){
        for(int j = 0;j < N;j ++){
            if(! brr[i][j] || (vis[i][j] == ff)) continue;

            q.push({-1, {i, j}}); vis[i][j] = ff;
            int d = 1; bool isCycle = 0;

            while(! q.empty() && !isCycle){
                int preDir = (q.front().first + 2) % 4;   /// dao nguoc lai
                int x = q.front().second.first, y = q.front().second.second;
                q.pop();

                for(int i = 0;i < 4;i ++){
                    if(i == preDir) continue;

                    int X = x + dx[i], Y = y + dy[i];

                    if(isInside(X, Y) && isKeNhau[brr[x][y]][brr[X][Y]][i]){
                        /// ke nhau
                        if(vis[X][Y] == ff){
                            isCycle = true;
                        } else {
                            d ++;   vis[X][Y] = ff;

                            q.push({i, {X, Y}});
                        }
                    }
                }
            }

         ///   cout << "??? " << i << " " << j << " " << isCycle << " " << d << "\n";

            if(!isCycle && (ans < d)){
                ans = d;
            }
        }
    }

    return ans;
}

void chuanhoa(string &nst){
    int x = bdX, y = bdY;

    for(char &i : nst){
        int huong;

        switch(i){
            case 'L' : {
                huong = 0;
                break;
            }
            case 'U' : {
                huong = 1;
                break;
            }
            case 'R' : {
                huong = 2;
                break;
            }
            case 'D' : {
                huong = 3;
                break;
            }
            default:{
                huong = 4;
                break;
            }
        }

        if(! isInside(x + dx[huong], y + dy[huong])){
            i = 'X';
        } else {
            x += dx[huong], y += dy[huong];
        }
    }
}

signed main(){


    ios_base::sync_with_stdio(false); cin.tie(0);

    cin >> N >> T;

    for(int i = 0;i < N;i ++){
        for(int j = 0;j < N;j ++){
            char c; cin >> c;

            if((c >= '0') && (c <= '9')) arr[i][j] = int(c - '0');
            else arr[i][j] = int(c - 'a') + 10;

            if(! arr[i][j]){
                bdX = i; bdY = j;
            }
        }
    }

    init();

    srand(time(0));
    /// sinh random NST
    for(int i = 0; i < maxNST;i ++){
        string s;

        int x = bdX, y = bdY;

        for(int j = 0;j < T;j ++){
            random_shuffle(idx, idx + 4);

            for(int k = 0;k < 4;k ++){
                int huong = idx[k];

                if(isInside(x + dx[huong], y + dy[huong])){
                    x += dx[huong]; y += dy[huong];

                    s = s + "" + dir[huong];

                    break;
                }
            }
        }

        chuanhoa(s);

        NST.push_back(s);
    }
    /// bat dau qua trinh
    /// chay den het tgian thi thoi
    while(1){
        sort(NST.begin(), NST.end(), [&](const string &n1, const string &n2){
                return calScore(n1) > calScore(n2);
        });


        if(t.elapsed_time() > Ti){
            for(char i : NST[0]){
                if(i != 'X') cout << i;
            }

            exit(0);
        }

        /// 15 thang tot nhat the he moi + 5 thang tot nhat the he cu
        priority_queue<pair<int, string> > pq;
        for(int i = 0;i < maxNST;i ++){
            for(int j = i + 1;j < maxNST;j ++){
                int index = 1 + rand() % T;

                string newNST = NST[i].substr(0, index)
                                + NST[j].substr(index);

                chuanhoa(newNST);

                pq.push({calScore(newNST), newNST});
            }
        }

        for(int i = 10;i < maxNST;i ++){
            NST[i] = pq.top().second; pq.pop();
        }
    }

    return 0;
}

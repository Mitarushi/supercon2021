#include "../main.cpp"
#include <queue>

int main(){
  read_graph();

  std::string s;
  std::cin>>s;

  if(s=="NO"){
    return 0;
  }

  int gieee;
  std::cin>>gieee;

  std::vector<int> gieee2(gieee);
  for(int i=0;i<gieee;i++){
    std::cin>>gieee2[i];
    gieee2[i]--;
  }

  std::vector<std::vector<int>> syuturyoku(n,std::vector<int>(n,-1));
  syuturyoku[0][0]=0;

  std::queue<std::pair<int,int>> que;
  que.emplace(0,0);

  while(!que.empty()){
    
  }
}
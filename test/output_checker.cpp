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
    auto[y,x]=que.front();
    que.pop();

    for(int opeope:gieee2){
      if(edge_map[opeope][y][x]==1){
        auto[move_y,move_x]=operation_shift[opeope];

        if(syuturyoku[y+move_y][x+move_x]==-1){
          que.emplace(y+move_y,x+move_x);
          syuturyoku[y+move_y][x+move_x]=syuturyoku[y][x]+1;
        }
      }
    }
  }

  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      if(obstacle_map[i][j]==1){
        std::cout<<"#"<<" ";
      }
      else{
        std::cout<<syuturyoku[i][j]<<" ";
      }
    }
    std::cout<<std::endl;
  }
}
#include<stdio.h>

int main(){
    int n,costMatrix[100][100],flag[100],dist[100],last[100],v;
    printf("Enter number of nodes:\n");
    scanf("%d", &n);
    printf("Enter cost Matrix:\n");
    for (int i = 0; i < n;i++){
        for (int j = 0; j < n;j++){
            scanf("%d", &costMatrix[i][j]);
            if(costMatrix[i][j] < 0){
                costMatrix[i][j] = 1000;
            }
        }
    }

    printf("Enter source node :\n");
    int srcr;
    scanf("%d", &srcr);

    for (int i = 0; i < n;i++){
        flag[i] = 0;
        last[i] = srcr;
        dist[i] = costMatrix[srcr][i];
    }
    flag[srcr] = 1;
    int min;
    for (int i = 0; i < n; i++)
    {
        min = 1000;
        for (int j = 0; j < n; j++)
        {
            if(!flag[j]){
                if(dist[j] < min){
                    min = dist[j];
                    v = j;
                }
            }
        }
        flag[v] = 1;

        for (int j = 0; j < n;j++){
            if(!flag[j]){
                if(min + costMatrix[v][j] < dist[j]){
                    dist[j] = min + costMatrix[v][j];
                    last[j] = v;
                }
            }
        }
    }

    for (int t = 0; t < n; t++)
    {
        printf("\nShortest path cost: %d", dist[t]);
    }

    printf("\n");
    return 0;
}

#include <stdio.h>
#include <string.h>

int main()
{
    int ar, srcr, t, j, k, w, v, min;
    int costmatrix[100][100], dist[100], last[100];
    int flag[100];

    printf("\nEnter the number of routers: ");
    scanf("%d", &ar);

    printf("\nEnter the distance matrix values:\n");
    for (int i = 0; i < ar; i++)
    {
        for (j = 0; j < ar; j++)
        {
            scanf("%d", &costmatrix[i][j]);
            if (costmatrix[i][j] < 0)
                costmatrix[i][j] = 1000;
        }
    }

    printf("\nEnter the source router: ");
    scanf("%d", &srcr);

    for (v = 0; v < ar; v++)
    {
        flag[v] = 0;
        last[v] = srcr;
        dist[v] = costmatrix[srcr][v];
    }

    flag[srcr] = 1;

    for (t = 0; t < ar; t++)
    {
        min = 1000;
        for (w = 0; w < ar; w++)
        {
            if (!flag[w])
            {
                if (dist[w] < min)
                {
                    v = w;
                    min = dist[w];
                }
            }
        }

        flag[v] = 1;

        for (w = 0; w < ar; w++)
        {
            if (!flag[w])
            {
                if (min + costmatrix[v][w] < dist[w])
                {
                    dist[w] = min + costmatrix[v][w];
                    last[w] = v;
                }
            }
        }
    }

    for (t = 0; t < ar; t++)
    {
        printf("\n%d -> %d: Path taken: %d", srcr, t, last[t]);
        printf("\nShortest path cost: %d", dist[t]);
    }

    printf("\n");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_data_len 400 //每筆raw data的最大長度
#define new_length 50 //經處理後data的固定長度
#define data_num 30 //data總數 : 隨著收更多資料需要更新這個數值
#define target_num 2 //output可能答案總數


void normalize(float* data, int length) // 將數列正規化到0到1之間
{
    float min = data[0];
    float max = data[0];
    int i;
    // 找出數列中的最小值和最大值
    for (int i = 1; i < length; i++) {
        if (data[i] < min) {
            min = data[i];
        }
        if (data[i] > max) {
            max = data[i];
        }
    }

    for (int i = 0; i < length; i++) {
        data[i] = (data[i] - min) / (max - min);
    }
} 

void scale_data(float (*data), int length) {
    int i;
    float factor = (float)(length - 1) / (new_length - 1);
    float *new_data;
    float max, min, mean;
    if((new_data=malloc(new_length * sizeof(float)))==NULL)
        printf("malloc error\n");
    for (i = 0; i < new_length; i++) {
        float index = i * factor;
        int lower_index = (int)index;
        int upper_index = lower_index + 1;
        float weight = index - lower_index;
        if (upper_index >= length) {
            new_data[i] = (data)[lower_index];
        } else {
            new_data[i] = (1 - weight) * (data)[lower_index] + weight * (data)[upper_index];
        }
    }
    // data正規化
    normalize(new_data,new_length);
    memcpy(data, new_data, new_length * sizeof(float));
    free(new_data);
}

int main() {
    int success_flag = 0;
    FILE *fp;
    int count;
    int i,j;
    int x_no=0,y_no,z_no,c;

    char filename[100]; //配給檔案路徑名稱記憶體，array大小隨你路徑長度調整

    char data_buf[20]; //從txt接入data的buffer

    // 創建dataset,並設定offset=0
    float train_data_input[data_num][new_length * 3];
    int train_data_output[data_num][target_num];
    memset(train_data_output, 0, sizeof(train_data_output));

    float datax[max_data_len],datay[max_data_len],dataz[max_data_len];

    for (i = 1; i <= data_num;i++)
    {
        
        sprintf(filename, "C:\\Users\\seank\\Desktop\\work\\ARDUINO_ML\\my_test\\train_data_2\\DATA%d.txt", i); //設定檔案路徑名稱
        fp = fopen(filename,"r");
        if(fp==NULL)
        {
            printf("open file fail\n");
            break;
        }
        printf("DATA%d 讀取中\n",i);
        count = 0;
        /*
        while (fgets(data_buf, 20, fp) != NULL)
        {
            if (count >= max_data_len) //判斷數據是否超過最大個數
            {
                printf("數據過多，無法全部讀取！\n");
                break;
            }
            
            char* token = strtok(data_buf, " \t"); // 使用空白和制表符作為分隔符
            c = 0;

            while (token != NULL)
            {
                if (c == 0)
                    y_no = atoi(token);
                else if (c == 1)
                    z_no = atoi(token);

                token = strtok(NULL, " \t");
                c++;
            }

            datax[count] = (float)y_no; // 將字符串轉換為浮點數並存儲到陣列中
            datay[count] = (float)z_no;
            count++;
        }
        */
        while(fgets(data_buf, 20, fp) != NULL)
        {
            if (count >= max_data_len) //判斷數據是否超過最大個數
            {
                printf("數據過多，無法全部讀取！\n");
                break;
            }
            c = 0;
            for(j=0;data_buf[j];j++)
            {
                if(data_buf[j]==' ')
                {
                    data_buf[j]='\0';
                    if(c==0)
                        y_no=j+1;
                    else
                        z_no=j+1;
                    c++;
                }                
            }
        
            datax[count] = atof(data_buf+x_no);//將字符串轉換為浮點數並存儲到陣列中
            datay[count] = atof(data_buf+y_no); 
            dataz[count] = atof(data_buf+z_no);
            count++; 
        }
        fclose(fp);

        //資料長度一致化;
        scale_data(datax, count);;
        scale_data(datay, count);
        scale_data(dataz, count);

        //設定train data input
        for (int j = 0; j < new_length; j++) 
        {
            train_data_input[i-1][j] = datax[j];
            train_data_input[i-1][j + new_length] = datay[j];
            train_data_input[i-1][j + 2*new_length] = dataz[j];
        }

        //標記train data output
        if(i% target_num ==0) 
        {
            train_data_output[i-1][ target_num-1 ]=1;
        }
        else
        {
            train_data_output[i-1][i% target_num -1]=1;
        }
        printf("DATA%d 資料寫入dataset成功\n", i);
        if(i==20)
            success_flag = 1;
    }
    
    for (i = 0; i < data_num; i++)
    {
        if(!success_flag)
            break;
        printf("\nDATA[%d] input:\n",i+1);
        for (j = 0; j < 3*new_length;j++){
            printf("%.2f ", train_data_input[i][j]);
            printf(",");
        }
        
        printf("\nDATA[%d] output:\n",i+1);
        for (j = 0; j < target_num;j++){
            printf("%d ", train_data_output[i][j]);
            printf(",");
        }
        
    }
    if(success_flag)
        printf("dataset建立成功\n");
    else
    {
        printf("dataset建立失敗\n");
    }
    return 0;
}

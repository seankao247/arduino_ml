.ino檔案是arduino

ARDUINO_ML資料夾內是在PC執行的code
1. 收集訓練data :(esp32執行arduino_ML_print_acceration.ino)
該code執行過程中會Print給電腦 (使用了一個Tera Term可以將終端機的資料直接存成log.txt)
訓練時手握加速度計adxl，掌心朝x方向，拳頭朝z方向
設定30筆訓練資料 每個攻擊和防禦各收集15筆

2. PC執行spilt_file.c 
這code 是用來把log.txt的檔案 分割為data1.txt ... dataN.txt ... data30.txt 
看demo資料夾 是最終成果 原本只會有log.txt

3. PC執行preprocess.c   
把每筆rawdata(dataN.txt)用插值法固定為同一大小 ex.50
output什麼??

4. PC執行training.c (也可以用esp32進行訓練 是用arduino_ML_training.ino)
忘記了???? 這邊要在看懂 

執行的時候也要看懂X[] 是做什麼的   忘記了????
是怎麼決定其值的
(weight print在終端機)

5.最後手動將訓練好的weight 寫進run_predict.ino
執行run_predict.ino




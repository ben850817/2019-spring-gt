# 2019-spring-gt
1.無向圖演算法
(a)找出圖上所有奇點，奇點數目必定為偶數個。
(b)找出奇點點對之間的最短路徑。
(c)對所有奇點做最小權重分配，權重使用點對之間的最小路徑長度。
(d)將匹配邊加在原圖上，此步驟可以消除原圖上的所有奇點，然後可以找出歐拉環。
(e)將歐拉環上的匹配邊轉為兩點間的最短路徑，即可得中國郵差路徑。

2.程式碼說明
Data Structure:
  (a)類別Node為圖的節點，Node會包含名稱，相連的邊，此點若為奇點則會得到一個編號。
  (b)類別Net為圖的邊，Net包含此邊的兩個端點，以及兩個bool變數，flag用來記錄尋找歐拉環時已經走過的邊，fake則表示匹配邊。

Function:
  (a)readFile為讀檔。
  (b)Min_W_Matching為最小權重匹配，最小權重匹配使用遞迴方式排列出所有匹配可能，function一開始會使用start變數作為起始點，從int i = start + 1
開始進行匹配，然後將被選中的奇點和start + 1做交換，然後進入下一個遞迴function重複上述動作，當start為奇點list的倒數第二個時，遞迴中止，若當下
的奇點排序方式的總體權重小於min_weight，則記錄此種排列方式。
  (c)Hierholzer為遞迴尋找歐拉環，會先找出任意一環，然後再從此環進行延伸。此function回傳節點陣列。
  (d)tracePath會print出歐拉環的function，此function會將歐拉環中的匹配邊轉為兩點間的最短路徑。
  (e)FindShortPath會找出節點間的最短路徑。
  (f)ShowShortPath會找出兩點間的最短路徑，回傳字串陣列，陣列中的字串代表節點名稱。

Container:
  (a)vlist為節點的list，ovlist為奇點的list，euler_cycle依序存放歐拉環通過的節點。
  (b)elist為邊的list。
  (c)length_matrix為奇點點對間的最短距離。
  (d)optimal_list為擁有最小權重匹配的奇點排序方式。
  (c)avail_path
  (d)shortpath
  (e)val_of_shortpath

Main:
  (a)一開始會先使用套件讀檔，然後呼叫readFile進行一般讀檔，建立需要的資料結構。
  (b)接下來會找出節點間的最短路徑。
  (c)計算奇點數目，變數ovindex為奇點的數目，若奇點數目不為0(if(ovindex != 0))，則會對奇點先進行最小權重匹配並且將匹配邊加入原圖，然後再尋找歐
拉環(euler_cycle = Hierholzer())。

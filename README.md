# じゃんけんトーナメントシミュレータ
このプログラムは、指定された数の参加者で行う一万回のじゃんけんトーナメントシミュレーションします。（参加者が2のべき乗でない場合はプレトーナメントを開催します。）各参加者にはランダムにじゃんけんの手が割り当てられ、勝者がトーナメントスタイルで次々に決定され、じゃんけんの総対戦回数が1万回に達するまでトーナメントを繰り返します。すべての対戦結果をresults.csvに記録し、各参加者の総勝利数と最終勝者を報告します。参加者とジャンケンの総対戦回数は任意の回数に変更可能です。

## 機能
・参加者数の入力  
・ランダムにじゃんけんの手を割り当て  
・トーナメント形式での勝者決定  
・各対戦の結果をresults.csvに記録  
・参加者の総勝利数とゲームの優勝者の表示  
・使用方法  プログラムをコンパイルします（例: c-rps.c）  
・生成された実行可能ファイルを実行します（例: ./a.out）  
・プロンプトに従って参加者の数を入力します  
・トーナメントが自動的に進行し、結果がresults.csvに記録されます    　
## 入力  
・プログラム実行時にコンソールから参加者の数を入力します  
## 出力  
・コンソールには各対戦の結果と総勝利数、ゲームの優勝者が表示されます   
・results.csvには、全ての対戦結果とプレイヤーの総勝利数、ゲームの優勝者が記録されます   
## 注意事項  
・このプログラムは、参加者が最大1000人、対戦回数の上限が10000回としています  
・これらの値は、プログラムの定数で調整可能です  
・results.csvファイルは、プログラムが実行されるたびに上書きされます  
・過去の結果を保持したい場合は、ファイルを別の場所に保存してください   
## Rock-paper-scissors tournament simulator  
This programme simulates a 10,000 rock-paper-scissors tournament with a specified number of participants. (If the number of participants is not a power of two, a pre-tournament is organised.) Each participant is randomly assigned a rock-paper-scissors move and the winners are determined one after the other in tournament style, repeating the tournament until the total number of rock-paper-scissors matches reaches 10 000. The results of all matches are recorded in results.csv, reporting the total number of wins and the final winner for each participant. The number of participants and the total number of rock-paper-scissors matches can be changed to any number.
## Functions.
Enter the number of participants.  
Randomly assigns rock-paper-scissors moves.  
Determines the winner in a tournament format.  
Record the results of each game in results.csv.  
Display of the total number of wins of the participants and the winner of the game.  
Compile the program (e.g. c-rps.c).  
Run the generated executable file (e.g. . /a.out).  
Enter the number of participants as prompted.  
The tournament proceeds automatically and the results are recorded in results.csv.  
## Input  
Enter the number of participants from the console when the programme is run.  
## Output
The console displays the results of each match, the total number of victories and the winner of the game.  
The results.csv file records the results of all matches, the total number of wins for the players and the winners of the games.  
## Notes.
This programme has a maximum of 1000 participants and a maximum of 10,000 games.  
These values can be adjusted using the program's constants.  
The results.csv file is overwritten each time the programme is run.  
If you want to keep past results, please save the file in a different location.  


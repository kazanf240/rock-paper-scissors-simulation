#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> // bool, true, falseを使用するために追加
#include <string.h> // 文字列関数を使用するために追加
#define MAX_PEOPLE 1000//最大人数
#define MATCH_LIMIT 10000 // 対戦回数の上限
#define RESULT_FILE "results.csv" // 結果を保存するファイル名
//じゃんけんの手を表す列挙体
enum hand{ ROCK, SCISSORS, PAPER};

//人を表す構造体
typedef struct{
    int id;
    enum hand choice;
    int wins; //"wins"は、それぞれのプレイヤーがトーナメントの中でどれだけ勝利したかを数えるための配列
    bool participated; // このフラグでプレトーナメントへの参加状況を追跡
}Person;

int totalMatches = 0;
int totalWins[MAX_PEOPLE] = {0}; // それぞれのプレイヤーの総勝利数を記録する配列 

//人にIDとジャンケンの手を割り当て、勝利数を初期化
void assignPersonDetails(Person *people, int n){
    for(int i = 0; i < n; i++){
        people[i].id = i +1;//peopleはPerson型のobjectの配列
        people[i].choice = rand()%3;
        people[i].wins = 0;
        people[i].participated = false; // Initialize participated flag here
    }
}

    
//手の名前を文字列で返す関数
const char* handToString(enum hand h){
    switch(h){
        case ROCK: return "グー";
        case SCISSORS: return "チョキ";
        case PAPER: return "パ―";
        default: return "不明";
    }
}

// 対戦結果をファイルに書き込む関数
void writeResultToFile(const char* filename, const char* result) {
    FILE* file = fopen(filename, "a"); // ファイルを追記モードで開く
    if (file != NULL) {
        fprintf(file, "%s\n", result); // 結果をファイルに書き込む
        fclose(file); // ファイルを閉じる
    } else {
        printf("ファイルを開けませんでした。\n");
    }
}

// CSVファイルにゲームの結果セクションを追加
void writeGameResultsHeader() {
    writeResultToFile(RESULT_FILE, "\nゲームの結果:");
    writeResultToFile(RESULT_FILE, "ルーム,対戦者,結果");
}

// CSVファイルにプレイヤーの総勝利数セクションを追加
void writeTotalWinsHeader() {
    writeResultToFile(RESULT_FILE, "\nプレイヤーID,総勝利数");
}

//じゃんけんの勝者を決定する関数
int judge(Person *player1, Person *player2, int room){
    if (totalMatches >= MATCH_LIMIT) {
        return -1; // 追加: 対戦回数が上限に達している場合は即座に処理を停止
    }


    totalMatches++;
    char result[1024]; // 対戦結果を格納する文字列
    while(player1->choice == player2->choice){
        if (totalMatches >= MATCH_LIMIT) { // 追加: 再抽選前にチェック
            return -1;
        }
        // あいこの場合の処理
        snprintf(result, sizeof(result), "ルーム%d: コンピューター%d(%s) vs コンピューター%d(%s) - あいこ",
                 room, player1->id, handToString(player1->choice), player2->id, handToString(player2->choice));
        printf("%s\n", result);
        writeResultToFile("results.csv", result);


        player1->choice = rand()%3;
        player2->choice = rand()%3;
        totalMatches++; // 再抽選で2回分の対戦としてカウント
    }

    // 勝敗が決まったら、その時の手を使って表示
    snprintf(result, sizeof(result), "ルーム%d: コンピューター%d(%s) vs コンピューター%d(%s) - ",
             room, player1->id, handToString(player1->choice), player2->id, handToString(player2->choice));

    if((player1->choice == ROCK && player2->choice == SCISSORS) ||
       (player1->choice == SCISSORS && player2->choice == PAPER) ||
       (player1->choice == PAPER && player2->choice == ROCK)){
        totalWins[player1->id - 1]++; // 総勝利数を更新
        snprintf(result + strlen(result), sizeof(result) - strlen(result), "勝者: コンピューター%d", player1->id);
        printf("%s\n", result);
        writeResultToFile("results.csv", result);
        return 1; //player1の勝ち
    } else {
        totalWins[player2->id - 1]++; // 総勝利数を更新
        snprintf(result + strlen(result), sizeof(result) - strlen(result), "勝者: コンピューター%d", player2->id);
        printf("%s\n", result);
        writeResultToFile("results.csv", result);
        return 0; //player2の勝ち
    }
}


//numOfPlayers以下の最大の２のべき乗を求める関数
int calculateMaxPowerOfTwo(int n){
    int result = 1;
    while(n > 1){
        n >>= 1;
        result <<= 1;
    }
    return result;
}

// プレトーナメントの処理を修正
void preTournament(Person *people, int *numOfPlayers) {
    if (totalMatches >= MATCH_LIMIT) return; // 追加: 開始前にチェック


    int targetPlayers = calculateMaxPowerOfTwo(*numOfPlayers);
    // プレトーナメントが必要な場合のみメッセージを表示
    if (*numOfPlayers > targetPlayers) {
        printf("プレトーナメントを実行します。\n");
    }

    
    Person tempPeople[MAX_PEOPLE]; // 一時配列を使用

    // 全参加者を一時配列にコピー
    for (int i = 0; i < *numOfPlayers; i++) {
        tempPeople[i] = people[i];
        tempPeople[i].participated = false; // participatedフラグをリセット
    }

    // プレトーナメントで勝者を決定
    int matchesNeeded = *numOfPlayers - targetPlayers;
    for (int i = 0; i < matchesNeeded; i++) {
        if (totalMatches >= MATCH_LIMIT) break;//追加: 各対戦前にチェック
        int idx1 = rand() % *numOfPlayers;
        int idx2 = rand() % *numOfPlayers;
        while (idx1 == idx2 || tempPeople[idx1].participated || tempPeople[idx2].participated) {
            idx1 = rand() % *numOfPlayers;
            idx2 = rand() % *numOfPlayers;
        }
        tempPeople[idx1].participated = tempPeople[idx2].participated = true;

        //勝者を決定するコードを挿入
        int winner = judge(&tempPeople[idx1], &tempPeople[idx2], i + 1);
        if (winner == -1) { // 追加: judge関数からの戻り値で対戦回数上限チェック
        break; // 対戦回数が上限に達した場合はループを抜ける
        }
        // 勝者をpeople配列に保存
        if (winner == 1) {
            people[i] = tempPeople[idx1];
        } else {
            people[i] = tempPeople[idx2];
        }
    }

    // プレトーナメントに参加しなかった残りの参加者をpeople配列に追加
    int count = matchesNeeded;
    for (int i = 0; i < *numOfPlayers && count < targetPlayers; i++) {
        if (!tempPeople[i].participated) {
            people[count++] = tempPeople[i];
        }
    }

    *numOfPlayers = targetPlayers; // 参加者数を更新
}



// トーナメントを進行する関数、参加者数を引数とする
void tournament(Person *people, int numOfPlayers) {
    int roundNumber = 1;
    for (int round = numOfPlayers; round > 1; round /= 2, roundNumber++) {
        if (totalMatches >= MATCH_LIMIT) { // ラウンド開始前にチェック
            return; // 10000回を超えていたらトーナメントを停止
        }

        printf("ラウンド%d:\n", roundNumber);
        for (int i = 0, room = 1; i < round; i += 2, room++) {//iを2つずつ増やしているのは、トーナメントの各対戦が2人のプレイヤーで行われるから
            if (totalMatches >= MATCH_LIMIT) { // Check after each match
                return; // Stop the tournament if totalMatches >= 20
            }
            int winner = judge(&people[i], &people[i + 1], room);
            
            if (winner) {
                people[i / 2] = people[i];
                people[i / 2].wins++;
            } else {
                people[i / 2] = people[i + 1];
                people[i / 2].wins++;
            }
        }
         
    }
}

// main関数の一部を修正

int main() {
    srand(time(NULL)); // 乱数の初期化
    int initialNumOfPlayers, numOfPlayers;

    printf("参加者の数を入力してください：");
    scanf("%d", &initialNumOfPlayers); // ユーザーからの入力を受け取る
    if (initialNumOfPlayers > MAX_PEOPLE) {
        initialNumOfPlayers = MAX_PEOPLE; // 最大人数を超える場合は調整
    }

    numOfPlayers = initialNumOfPlayers; // 初期の参加者数を設定
    Person people[MAX_PEOPLE];

    // CSVファイルを開き、ヘッダーを書き込む
    FILE* file = fopen(RESULT_FILE, "w");
    if (file != NULL) {
        fprintf(file, "ルーム,対戦者,結果\n");
        fclose(file); // ここで一旦閉じる
    } else {
        printf("ファイルを開けませんでした。\n");
        return 1;
    }

    while (true) {
        assignPersonDetails(people, numOfPlayers); // 参加者の詳細を割り当て

        if (numOfPlayers & (numOfPlayers - 1)) {
            preTournament(people, &numOfPlayers); // 2のべき乗でない場合、プレトーナメントを実施
        }

        tournament(people, numOfPlayers); // トーナメントを実行

        if (totalMatches >= MATCH_LIMIT) {
            char matchSummary[256];
            sprintf(matchSummary, "\nゲーム終了。総対戦回数: %d", totalMatches );
            printf("%s\n", matchSummary);
            writeResultToFile(RESULT_FILE, matchSummary); // 総対戦回数もファイルに記録
            break; // 総対戦回数が20に達したらループから抜ける
        } else{
        // 総対戦回数が20未満であれば、参加者の状態をリセットして再びトーナメントを開始
            for (int i = 0; i < numOfPlayers; i++) {
                people[i].wins = 0; // 勝利数をリセット
                people[i].participated = false; // 参加状況をリセット
            }
            numOfPlayers = initialNumOfPlayers;
        }
        
    }

    // プレイヤーの総勝利数セクションをCSVファイルに追加
    writeTotalWinsHeader();
    for(int i = 0; i < initialNumOfPlayers; i++) {
        char resultString[256];
        sprintf(resultString, "コンピューター%d,%d", i + 1, totalWins[i]);
        writeResultToFile(RESULT_FILE, resultString);
    }

    // 総勝利数をターミナルに表示
    printf("\nプレイヤーの総勝利数:\n");
    for(int i = 0; i < initialNumOfPlayers; i++) {
        printf("コンピューター%dの総勝利数: %d\n", i + 1, totalWins[i]);
    }


    // 最多勝利数とそのプレイヤーを探す
    int maxWins = 0;
    int maxWinsPlayerId = 0;
    for (int i = 0; i < initialNumOfPlayers; i++) {
        if (totalWins[i] > maxWins) {
            maxWins = totalWins[i];
            maxWinsPlayerId = i + 1; // プレイヤーIDは1から始まるので、インデックスに1を足す
        }
    }

    // ターミナルに最も勝利数が多いプレイヤーを表示
    printf("ゲームの優勝者: コンピューター%d, 総勝利数: %d\n", maxWinsPlayerId, maxWins);

    // CSVファイルに最も勝利数が多いプレイヤーを追加
    writeResultToFile(RESULT_FILE, "\nゲームの優勝者:");
    char winnerResult[256];
    sprintf(winnerResult, "コンピューター%d,%d", maxWinsPlayerId, maxWins);
    writeResultToFile(RESULT_FILE, winnerResult);


    printf("結果を%sに書き出しました。\n", RESULT_FILE);
    return 0;
}
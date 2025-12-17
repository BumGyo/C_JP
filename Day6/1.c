/* スペルチェッカー

      テキストファイル（英文テキストでもプログラムでもかまわない，ただ
      し日本語文字は入っていないと仮定する）の中に現れる単語のうち，
      辞書に登録されていないものの一覧表を出力する。出力は，該当する単
      語だけを辞書式順序（アルファベット順）に並べて行う。

      このプログラムは，二つのファイルを読み込む。第 1 のファイルが辞
      書，第 2 のファイルが目的のテキストファイルである。第 1 のファイ
      ル中の単語すべてをチェック表に記録した後で，第 2 のファイルを読
      み込む。このファイルの中の単語もすべてチェック表に記録するが，辞
      書の中にあった単語か否かが後で分かるように，チェック表の中に印を
      つけておく。

      二つのファイルを読み終わった後で，チェック表を調べ，辞書には現
      れずテキストファイルだけに現れた単語を集め，整列してから，出力
      する。

      単語とは，英字（アルファベット）だけからなる文字の列である。単語
      の前後は，英字以外の文字であれば，改行，空白，記号，数字その他何
      でもかまわない。辞書もテキストファイルも，単語以外の文字は無視す
      る。たとえば，1 行あたり単語一つの形式でもかまわないし，1 行に数
      多くの単語を詰め込んでもかまわない。ただし，辞書の中に同じ単語が
      2 回以上現れた場合はエラーと見なす。英字の大文字と小文字は同一
      視する。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max_Length  20      /* 一つの単語の文字数の上限 */
#define Table_Size  127     /* チェック表の大きさ，単語の種類の数の上限 */

/* チェック表のデータ構造

      チェック表は配列 check_table である。特定の単語の登録の有無を
      調べる場合，配列の先頭から調べ始め，探している単語と等しいつづりを
      持つ場所にぶつかるまで，調べる位置を一つずつずらしていく（添字を 1
      ずつ増やしていく）。変数 word_count が単語の個数を表す。

      チェック表のそれぞれの要素は，spell と flag の二つの要素からなる。
      spell には，単語の文字列（つづり）を入れる。flag は，その単語の
      性質を表す値（Dictionary，Text_File の 2 種類）である。Dictionary
      は，単語が辞書の中で見つかったことを示す（テキストファイルの中にあった
      か否かは問わない）。Text_File は，単語がテキストファイルの中でだけ
      見つかったことを示す。

      つづりの最後に 0 を入れて印としているのに注意。C では，文字列の最
      後を 0 で示すのが慣例となっている。これに従った方が，文字列操作
      関数などが使えて，何かと好都合である。配列 spell（や read_in の
      中の配列 word）の長さが Max_Length+1 になっているのは，印 0 を入れ
      る場所が余分に一つ必要だからである。

      typedef で，spell と flag の二つの要素からなる構造体に item とい
      う名前を付けている（型宣言している）。checktable は，item 型のデー
      タが並んだ配列である。
*/

typedef enum { Dictionary, Text_File } kind;

typedef struct {
    char spell[Max_Length+1];
    kind flag;
} item;

item checktable[Table_Size];
int word_count;

/* 関数のプロトタイプ */

void read_in(FILE *f, kind phase);
void clear_checktable(void);
void insert_checktable(char *word, kind phase);
void print_out(void);
int pack_checktable(void);
void sort_checktable(int count);

/* 主プログラム */

int main(int argc, char *argv[]) {
    FILE *f;

    if (argc != 3) {
        printf("引数の個数は二つでなければなりません\n");
        exit(1);
    }
    clear_checktable();
    f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("%s: 読むことができません\n", argv[1]);
        exit(1);
    }
    read_in(f, Dictionary);
    fclose(f);
    f = fopen(argv[2], "r");
    if (f == NULL) {
        printf("%s: 読むことができません\n", argv[2]);
        exit(1);
    }
    read_in(f, Text_File);
    fclose(f);
    print_out();
    return (0);
}

/* ファイルの読み込み

      phase = Dictionary であれば辞書
      phase = Text_File  であればテキストファイル
*/

void read_in(FILE *f, kind phase) {
    int  c, k;
    char word[Max_Length+1];

    c = getc(f);
    while (c != EOF) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            k = 0;
            while ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                if (k >= Max_Length) {
                    printf("単語の長さが長過ぎます\n");
                    exit(1);
                }
                if (c <= 'Z')
                    c += 'a'-'A';
                word[k++] = c;
                c = getc(f);
            }
            word[k] = 0;
            insert_checktable(word, phase);
        }
        else
            c = getc(f);
    }
}

/* チェック表の初期化 */

void clear_checktable(void) {

    word_count = 0;
}

/* 単語をチェック表に入れる */

void insert_checktable(char *word, kind phase) {
    int  h;

    h = 0;
    while (h < word_count) {
        if (strcmp(word, checktable[h].spell) == 0) {
            if (phase == Dictionary) {
                printf("辞書中に同じ単語が複数個存在します\n");
                exit(1);
            }
            return;
        }
        h++;
    }
    if (word_count == Table_Size) {
        printf("単語の種類が多過ぎます\n");
        exit(1);
    }
    strcpy(checktable[word_count].spell, word);
    checktable[word_count].flag = phase;
    word_count++;
}

/* プログラムの結果を出力する */

void print_out(void) {
    int  count, k;

    count = pack_checktable();
    sort_checktable(count);
    for (k = 0; k < count; k++)
        printf("%s\n", checktable[k].spell);
}

/* 目的とする単語だけを表の前の方に詰める

      チェック表の中には辞書ファイルに現れた単語とテキストファイルに
      のみ現れた単語が含まれるが，出力すべき単語は後者のみである。
      そこで，この関数では，辞書ファイルに現れた単語を捨てて，出力す
      べき単語（テキストファイルにのみ現れた単語）だけを表の前の方に
      詰める。出力すべき単語の個数を関数の返り値として返す。
*/

int pack_checktable(void) {
    int  k = 0, h;

    for (h = 0; h < word_count; h++) {
        if (checktable[h].flag == Text_File){
            checktable[k] = checktable[h];
            k++;
        }
    }
    return k;
}

/* 整列

      単語をアルファベット順に整列する。アルゴリズムは単純な選択法（直
      接選択法とも言う）である。

      選択法のアルゴリズムを言葉による説明を交えて書くと，次のようにな
      る。ここでは，データの個数を n，整列する対象の配列を a とする。

          for (k = 0; k < n; k++) {
              1. 添字の範囲 k 〜 n-1 における a の最小値を探し，
                 その位置 m を求める（a[m] が最小値である）;
              2. a[k] と a[m] を入れ換える（交換する）;
          }
*/

void sort_checktable(int count) {
    int  k, j, m;
    item tmp;

    for (k = 0; k < count; k++) {
        m = k;
        for (j = k+1; j < count; j++){
            if (strcmp(checktable[j].spell, checktable[m].spell) < 0)
                m = j;
        }
        tmp = checktable[k];
        checktable[k] = checktable[m];
        checktable[m] = tmp;
    }
}

/* ライブラリ関数に関する注意

      このプログラムでは，二つのライブラリ関数 strcmp と strcpy を使っ
      ている。いずれも，文字型配列へのポインタ（配列の先頭を指す）二
      つを引数とする。

      strcmp は，二つの文字列を辞書式に比較する。第 1 引数が第 2 引数
      より小さければ負の値，大きければ正の値，等しければ 0 を返す。

      strcpy は，文字列をコピーする。第 2 引数の文字列を第 1 引数の場
      所にコピーする。
*/

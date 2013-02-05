#ifndef _SCENE_HPP_
#define _SCENE_HPP_

class GameResult;

// 各ゲームや各メニュー画面は、utils.hppに定義されているSceneクラスを
// 継承している必要がある。
// - 当該ゲームを一から始める際に呼ばれるメソッド（init）には
//   GameResultクラスのインスタンスが渡される。
// - データ更新メソッド（update）が、ゲーム結果としてGameResultクラスの
//   インスタンスへのポインタを返すと、当該ゲームの終了とみなされる。
//   なお、updateでゲームを終了しない場合には、NULLを返せばよい。

class Scene{
public:
	// [当該ゲームを一から始める際に呼ばれるメソッド]
	// コンストラクタとは別に定義して下さい。
	// - コンストラクタ：同一種類のゲームであれば1回呼べばよい操作（画像のロードなど）
	// - initメソッド：1ゲーム始めるごとに呼ばれるべき操作（フィールドの状態のリセットなど）
	// GameResultに、選ばれたステージ色および、現時点でのプレイヤーの各色の得点が格納されています。
	virtual void init(GameResult * result) = 0;

	// [データ（ロジック）を更新するメソッド]
	// 現時点では、呼ばれるタイミングは不定（updateとdrawを呼び出し後、
	// WaitTimer(10)したのちに再度updateとdrawを呼ぶ）です。
	// 特定のfpsごとに呼ぶ機構を導入するかも？
	// ここで返り値がNULLだとゲームが継続し、GameResultインスタンスへの
	// ポインタを返すとゲームが終了します。
	virtual GameResult * update(void) = 0;
	
	// [描画を行うメソッド]
	// updateが呼ばれた後、直後にこのメソッドの実行に移ります。
	// （updateの返り値がNULLでない場合は除く）
	virtual void draw(void) = 0;
};

// ゲームの結果や、メニュー画面におけるステージ選択の結果を引き渡すクラス
class GameResult{
	// [ステージID]
	// メニュー画面で選ばれたゲームのID（マップ上の1点につき
	// 1つ割り当てられるID）が格納される。ゲーム内から参照する
	// 必要はない。
	int stage_id_;
	
	// [マップID]
	// どのマップを使っているかを示す。
	// （チュートリアル選択用マップ、メインマップなど）
	int map_id_;
	
	// [シチュエーションID]
	// いまどのシーンなのかが格納される。
	// 0: メニューで選択中
	// 1: ゲーム開始前
	// 2: ゲーム中
	// 3: ゲーム後
	int stage_situation_;
	static const int STAGE_SITUATIONS = 4;
	
	// [ステージのレベル]
	// メニュー画面で選ばれたゲームのレベルが格納される。
	int stage_level_;

	// [ステージの色]
	// メニュー画面で選ばれたゲームの色が格納される。
	int stage_color_;
	
	// [ゲームをプレイした結果]
	// ゲーム終了時、この値にゲーム成功ならば1、失敗ならば0を
	// 指定して返す。
	int end_status_;
	
	// [赤の得点]
	// この値に、現時点でのプレイヤーの赤の得点（累積値）が
	// 格納された状態でゲームに渡される。
	// ゲーム終了時、この値にこのゲーム中でプレイヤーが獲得した
	// 赤の得点（上記の値に足した結果ではない）を指定して返す。
	int r_score_;

	// [緑の得点]
	// 赤と同様
	int g_score_;

	// [青の得点]
	// 赤と同様
	int b_score_;
	
public:
	GameResult() :
	  stage_id_(0), map_id_(0), stage_situation_(0), stage_level_(0), stage_color_(0), end_status_(0), r_score_(0), g_score_(0), b_score_(0) {}
	
	int stage_id(){ return stage_id_; }
	void stage_id(int si){ stage_id_ = si; }
	int map_id(){ return map_id_; }
	void map_id(int mi){ map_id_ = mi; }
	int stage_situation(){ return stage_situation_; }
	void stage_situation(int ss){ stage_situation_ = ss; }
	int stage_level(){ return stage_level_; }
	void stage_level(int sl){ stage_level_ = sl; }
	int stage_color(){ return stage_color_; }
	void stage_color(int sc){ stage_color_ = sc; }
	int end_status(){ return end_status_; }
	void end_status(int st){ end_status_ = st; }
	int r_score(){ return r_score_; }
	void r_score(int r){ r_score_ = r; }
	int g_score(){ return g_score_; }
	void g_score(int g){ g_score_ = g; }
	int b_score(){ return b_score_; }
	void b_score(int b){ b_score_ = b; }
};

#endif // _SCENE_HPP_

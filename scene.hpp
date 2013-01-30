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
	// ゲーム画面がGameResultを受け取ったとき：特に参照する必要なし
	// ゲーム画面がGameResultを返すとき：特に変更する必要なし
	// メニュー画面がGameResultを受け取ったとき：特に参照する必要なし
	// メニュー画面がGameResultを返すとき：次にプレイするゲームを格納する
	int stage_id_;

	// [ステージのレベル]
	// ゲーム画面がGameResultを受け取ったとき：メニュー画面で選ばれたゲームのレベルを示す
	// ゲーム画面がGameResultを返すとき：特に変更する必要なし
	// メニュー画面がGameResultを受け取ったとき：特に参照する必要なし
	// メニュー画面がGameResultを返すとき：特に変更する必要なし
	int stage_level_;

	// [ステージの色]
	// ゲーム画面がGameResultを受け取ったとき：メニュー画面で選ばれたゲームの色を示す
	// ゲーム画面がGameResultを返すとき：扱い未定
	// メニュー画面がGameResultを受け取ったとき：特に参照する必要なし
	// メニュー画面がGameResultを返すとき：特に変更する必要なし
	int stage_color_;

	// [ゲームをプレイした結果]
	// ゲーム画面がGameResultを受け取ったとき：特に参照する必要なし
	// ゲーム画面がGameResultを返すとき：ゲームをプレイした結果を返す（成功: 1, 失敗: 0）
	// メニュー画面がGameResultを受け取ったとき：特に参照する必要なし
	// メニュー画面がGameResultを返すとき：特に変更する必要なし
	int end_status_;

	// [赤の得点]
	// ゲーム画面がGameResultを受け取ったとき：現時点でのプレイヤーの赤の得点（累積値）を示す
	// ゲーム画面がGameResultを返すとき：このゲームで獲得した赤の得点を格納する
	// メニュー画面がGameResultを受け取ったとき：選べるステージを決定するのに利用する
	// メニュー画面がGameResultを返すとき：特に変更する必要なし
	int r_score_;

	// [緑の得点]
	// 赤と同様
	int g_score_;

	// [青の得点]
	// 赤と同様
	int b_score_;
	
public:
	GameResult() :
	  stage_id_(0), stage_level_(0), stage_color_(0), end_status_(0), r_score_(0), g_score_(0), b_score_(0) {}
	GameResult(int si, int sl, int sc, int st, int r, int g, int b) :
	  stage_id_(si), stage_level_(sl), stage_color_(sc), end_status_(st), r_score_(r), g_score_(g), b_score_(b) {}
	
	int stage_id(){ return stage_id_; }
	void stage_id(int si){ stage_id_ = si; }
	
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

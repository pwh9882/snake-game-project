# C++ 기말 프로젝트를 위한 repo

## 수정목록

|수정날짜|대표수정자|Revision|추가/수정 항목|내용|
|------|---|---|---|---|
|23-05-18|박우혁|0.0|최초작성|readme와 demogame|

## demogame

demo-game 폴더 안의 make파일을 실행한 결과물 demo-game을 실행하면
1단계 Map의 구현
2단계 Snake 표현 및 조작
3단계 Item 요소의 구현
위 3단계를 간단하게 구현한 demo-game을 실행해 볼 수 있습니다.

## snake-game 빌드 방법

game 폴더로 cd 한 후 make로 빌드. root 폴더에 snack-game이 생성됩니다.

## 목표

<1단계:>
1-1. Snack map을 21x21의 2차원 배열로 구성한다. 값이 의미하는 바는 다음과 같이 정한다.  
    nothing: 0
    snack_head: -1
    snack_body: 1~INT_MAX;
    Immune Wall: -2
    Wall: -3
    Gate: -4
    Growth: -15
    Poison: -25  

1-2. Ncurses를 이용하여 해당 2차원 배열 값을 읽어서 화면에 출력한다.  
1-3. Snack map의 크기를 다양화 하고, 여러 맵을 사용할 수 있도록 구조를 수정한다.  

<2단계:>  
2-1. 1단계의 맵 위에 Snake를 표시하고, 화살표를 입력 받아 Snake가 움직이도록 프로그램을 완성한다.  
2-2. Snake는 규칙 #1을 준수해야 한다.  
2-3. Tick에 대한 변화 주기(추가사항)  

<3단계:>  
3-1. 2단계 프로그램에서, Map위에 Growth Item와 Poison Item을 출현하도록 수정한다.  
  ○ 게임 규칙#2를 준수해야 한다.  
3-2. Growth Item과 Poison Item을 Map 배열에 표현할 때 값을 정한다.  
○ 화면상에 표현시, 색이나 기호를 달리하여 구분할 수 있도록 한다.  
○ Map Data에서 Growth Item은 5, Poison Item은 6과 같이 구분할 수 있도록 한다.  
3-3. Snake 와 Item 의 상호작용 처리 + Item의 종류에 대한 추가 (추가 사항): 새로운 Item에 대해 제안하고, 구현하기.  

<4단계:>  
4-1. 3단계 프로그램에서, Map의 Wall의 임의의 위치에 한 쌍의 Gate가 출현할 수 있도록 변경하고, 각 Gate에 Snake가 통과할 수 있도록 수정한다.  
  ● 게임 규칙 #3, #4, #5를 준수해야한다.  
  ● Wall(Immune Wall 포함)과 Gate를 Map 배열에 표현할 때 값을 결정한다.  
    ○ 화면상에 표현시, Gate는 Wall과 구분될 수 있도록한다.  
    ○ Map Data에서 Gate는 7과 같이 하여, 다른 요소와 구분할 수 있도록 한다.  
4-2. (1) Snake  Wall 의 상호작용 처리 (2) SnakeGate 의 상호작용 처리  
  (3) Wall에 대한 변화 추가 (추가 사항): 새로운 Wall 의 동작에 대해 제안하고, 구현하기  

<5단계:>  
5-1. 4단계 프로그램에서, 우측에 게임 점수를 표시하는 화면을 구성한다.  
  ○ 슬라이드 Snake Game의 구조 참고  
  ● 게임 점수는 게임 규칙 #6을 준수한다.  
5-2. Mission  
  ○ 1. 구성한 Map의 정의에 고정 값을 주거나,  
  ○ 2. 매 게임마다 임의의 값을 주는 방식으로 처리한다.  
  ● Mission을 달성하면 다음 Map으로 진행하도록 프로그램을 완성한다.  
  ○ Stage는 최소 4개로 구성하고, 각 Stage의 Map은 서로 달라야 한다.  
5-3. Score Board의 대한 설계사항  
  ○ (1) Game 상태를 Score Board로 표현  
  ○ (2) Score Board에 대한 변경 (추가 사항)  
  ■ Score Board에 대한 상호작용방법, 혹은  
  ■ Score Board에 표현되는 요소의 추가 등을 제안하고, 구현하기.  

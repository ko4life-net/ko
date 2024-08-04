* 맵툴에서 shape 오브젝트를 바꿀때 주의점
  - 이미 배치해놓은 오브젝트의 모델링 파일(shape)을 바꿀 필요가 있을때
	save한 폴더의 하위폴더인 mesh폴더에 있는 shape파일을 바꾸어주기만 하면
	맵상에 배치된 같은 shape파일을 쓰는 것들은 모두 바뀐다.
	하지만 source폴더의 shape도 바꾸어주어야 한다.(반드시..)

* game data 파일 확장자 설명
  - *.gmd : game map data, 아직 아무 내용없음
  - *.gtd : game terrain data, 지형 데이타파일
  - *.god : game object data, 지형에 배치된 object정보(object이름과 위치 회전 스케일값이 저장된 형식)
  - *.grm : game river main, 맵상에 보이는 모든 강의 정보
  - *.grp : game river patch, 맵의 한 구역(현재 128m) 안에 있는 강의 점정보들

* map editor save 파일 확장자 설명
  - *.n3m : n3 map eiditor file,
  - *.rvr : river data, 강 정보들
  - *.sdt : shape data text, object(shape)의 위치 회전 스케일 정보가 text형식으로 되어 있어서 직접 손으로 수정 가능
  - *.trn : terrain data, 지형정보가 들어있다.

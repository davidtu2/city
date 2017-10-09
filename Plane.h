class Plane{
public:
  Plane(int size):_size(size), _block(10.0f){
	_building1 = new Texture("textures/building.jpg");
	_building2 = new Texture("textures/building2.jpg");

	/*Proceduaral generation reference: 
	https://bitbucket.org/whleucka/cpsc-graphics-final/src/856ef81f67cf92f90c84368965331069d2de4e0f/src/main.cpp?at=master&fileviewer=file-view-default*/
	for(int j = -2; j > -_size - 6; j -= 6){
		//101 iterations. This is the x value
		for(int i = 0; i < _size + 6; i += 2){
			if(i % 12 != 10 && i % 12 != 0){//x value = {2, 4, 6, 8}
				/*randomSize = [1, 2]
				(We are adding 1 because we don't want 0 size/height)*/
				float randomSize = rand() % 2 + 1;
				float randomHeight;
				if(rand() % 5 + 1 == 1){
					//randomHeight = [1, 12] (Make a taller building)
					randomHeight = rand() % 12 + 1;
				}else{
					//randomHeight = [1, 7] (Make a shorter building)
					randomHeight = rand() % 7 + 1;
				}
				int randomTexture = rand() % 2;
				if(randomTexture == 0){
					_building = new Building(i, 
						0.0f, 
						j, 
						randomSize, 
						randomHeight, 
						_building1->getTexture());
				}else{
					_building = new Building(i, 
						0.0f, 
						j, 
						randomSize, 
						randomHeight, 
						_building2->getTexture());
				}
				printf("The random texture is %d.\n", randomTexture);
				_buildings.push_back(_building);
			}
		}
	}
  }

  virtual ~Plane(){
 	_buildings.clear();
	delete _building;
  }

  /*Start by drawing the blocks
  (The regions where the buildings will sit on top of)*/
  void draw(){
	glColor4f(0.0, 1.0, 0.0, 1.0f);
	glBegin(GL_QUADS);//Start drawing a 17 x 17 quadrilateral
	for(int j = 0; j < _size; j += 12){//Go to one row
		for(int i = 0; i < _size; i += 12){//Draw all the "columns" of the row
			//Bottom Left
			glVertex3f(0.0f + i, 0.0f, 0.0f - j);
			//Bottom right
			glVertex3f(0.0f + _block + i, 0.0f, 0.0f - j);
			//Top right
			glVertex3f(0.0f + _block + i, 0.0f, 0.0f - _block - j);
			//Top left
			glVertex3f(0.0f + i, 0.0f, 0.0f - _block - j);
		}
	}
	glEnd();

	glColor4f(0.0, 0.0, 1.0, 1.0);//Now draw the outer boundaries
	glBegin(GL_LINES);//Start drawing lines. Let's start with the left boundary
	
	//Bottom left corner of the map
	glVertex3f(-2.0f, 0.0f, 2.0f);
	//Top left corner of the map
	glVertex3f(-2.0f, 0.0f, -_size - 8);
	
	/*Next, let's do the back boundary. 
	Continuing from where we left off, this is the top left corner of the map*/
	glVertex3f(-2.0f, 0.0f, -_size - 8);
	//Top right corner of the map
	glVertex3f(_size + 8, 0.0f, -_size - 8);

	//Right boundary: top right corner of the map
	glVertex3f(_size + 8, 0.0f, -_size - 8);
	//Bottom right corner of the map
	glVertex3f(_size + 8, 0.0f, 2.0f);

	//Front boundary: bottom right corner of the map
	glVertex3f(_size + 8, 0.0f, 2.0f);
	//Back to where we started: the bottom left corner of the map
	glVertex3f(-2.0f, 0.0f, 2.0f);

	glEnd();

	//Draw Buildings
	for(std::vector<Building*>::iterator it = _buildings.begin(); it != _buildings.end(); ++it){
		(*it)->draw();
	}
  }

private:
  int _size;
  float _block;//Size of the block (a.k.a. the length of the "street")
  std::vector<Building*> _buildings;
  Building* _building;//Building to be inserted into XZ's vector
  Texture* _building1;
  Texture* _building2;
};

class Plane{
public:
  Plane(int size):_size(size), _block(10.0f){
	Texture* _texture = new Texture("textures/building.jpg");
	Texture* _texture1 = new Texture("textures/building2.jpg");
	_textures.push_back(_texture);
	_textures.push_back(_texture1);
	//In the future, add more textures here
	int buildingCount = 0;
	int randomTexture = 0;

	/*City Model reference: spawnBuildings() from:
	https://bitbucket.org/whleucka/cpsc-graphics-final/src/856ef81f67cf92f90c84368965331069d2de4e0f/src/main.cpp?at=master&fileviewer=file-view-default*/

	/*Start at -2 depth because we want to skip over the street
	We are going negative because we want to make the buildings along negative z
	"j" will be the z values for the buildings*/
	for(int j = -2; j > -_size - 6; j -= 6){
		//101 iterations. "i" will be the x values for the buildings
		for(int i = 0; i < _size + 6; i += 2){		
			/*Let's start randomizing the heights and sizes of the buildings.
			Since the above for loop increments by 2,
			this checks if [0, 2, 4, 6, 8, 10] is not equal to 0 AND 10
			We do not want 0 or 10 because we don't want to draw buildings on the street
			Even though 10 is within range of the block, the building can extend onto the street*/
			if(i % 12 != 10 && i % 12 != 0){
				/*randomSize can be [1, 2]
				Note that we are adding 1 because we don't want 0 size or height*/
				float randomSize = rand() % 2 + 1;

				float randomHeight;
				if(rand() % 5 == 0){
					//randomHeight can be [1, 25] (Make a taller building)
					randomHeight = rand() % 25 + 1;
				}else{
					//randomHeight can be [1, 5] (Make a shorter building)
					randomHeight = rand() % 5 + 1;
				}

				//Randomize my textures
				if(buildingCount == 4){
					/*For every 4 buildings, change the texture for the next set.
					Note that for every block, there are 2 sets of buildings, 
					so each block will have eight buildings*/
					buildingCount = 0;
					randomTexture = rand() % 2;
				}

				buildingCount++;

				if(randomTexture == 0){
					Building* _building = new Building(i,//x will be [2, 4, 6, 8]
						j,//Starting at -2, z will be increments of 6 in the negative z
						randomSize,//Can be [1, 2]
						randomHeight,//Can be [1, 25]
						_textures[0]->getTexture());
					_buildings.push_back(_building);
				}else{
					Building* _building = new Building(i,
						j,
						randomSize,
						randomHeight,
						_textures[1]->getTexture());
					_buildings.push_back(_building);
				}
			}
		}
	}
  }

  virtual ~Plane(){
 	_buildings.clear();
	_textures.clear();
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
  std::vector<Texture*> _textures;
};

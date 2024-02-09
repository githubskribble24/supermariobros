# Definitely Not a Super Mario Bros Clone

## Usage
You can navigate the main menu using the "W", "S" & "Spacebar" keys.
W = Up
S = Down
Spacebar = Choose level

Game keys:
W = Jump
A = Left
D = Right
Spacebar = Shoot

```
git clone https://github.com/githubskribble24/supermariobros.git
1. Open the .SLN file, build and run the program

If you want to play the game using the .exe file do the following steps as well:
1. Add the "fonts/", "images/", "levels/" & "assets.txt" file into the same place as the .exe

```

### Add a new texture
Inside of "assets.txt" you can add your texture like this
```
Texture     TEXTURENAME				TEXTUREPATH.png
Animation   ANIMATIONNAME			TEXTURENAME	1   1
```
The capital words you can fill in with your own information. The "1 1" at the end of the animation is the amount of 
textures there are in the texture file and at how many frames to move to the next texture. 
With the example it means: there is 1 texture move to the next texture every 1 frame.

### Add a new level
**For this part some C++ knowledge is needed to be able to add the .txt to the list of level files**

```
1. Go inside of the levels folder.
2. Create a new file with any name you like
3. Add the player information, add the blocks, block coordinates and the block textures
Example:
"
Player 2 6 48 48 4 -20 20 0.75 Buster
Tile Ground 10 10
Dec	 Cloud 10 12
Tile Pole  20 0
"
```
This will create a player at the X coords: 2 and Y coords: 6
Player X bounding box: 48px, Y bounding box: 48px
Player velocity: 4px per frame
Player jump velocity: -20px (because 0,0 is at the top left of the window we use -20 to jump up 20 pixels)
Player max speed in every direction: 20px;
Player gravity: 0.75
Player shooting texture: Buster

The other lines will be in this format:
Type ANIMATIONNAME XCoords YCoords

The lines that begin will "Tile" will have collisions this means that if you hit this block you will be stopped and you can't move into them
The lines that begin with "Dec" are just decoration these have no collision so you don't have to worry about hitting these.
If you hit the Tile Pole with your player the game will end and go back to the main menu.

After you have created a new level file you can go into **Scene_Menu.cpp** and add the level path into the **m_levelPaths vector**

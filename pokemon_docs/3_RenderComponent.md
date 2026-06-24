Now with  a basic game loop running and movement on the map is happening with basic rendering boxes. Now is a good time to add actual images (sprites,tiles, etc) on the map. 
Since the rendering resources will depend on the renderer we use (intuitively, since maybe another renderer expect another for of input), we add the resources in renderer interface. 

Each unique resource should be loaded into the memory once (Repository). Furthermore it makes sense to just store the metadata and read the texture parts with SFML using path and region each time some part of the texture needs to be loaded. The whole texture of the game is (for now) divided into:
1. Sprites : the entities on the map
2. Tiles: The background of each  tile 
3. MapObjects: The overlay objects with features
![[3_RenderComponent.svg]]

### Resources
Assets are very important!
- Pokemon Mystery Dungeon sprites: https://sprites.pmdcollab.org/
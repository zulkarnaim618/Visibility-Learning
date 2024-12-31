# Terms Description  


## data_gen
cellXCount = number of cell horizontally  
cellYCount = number of cell vertically  
cellX = width of each cell  
cellY = height of each cell  


## Open buildings 2D:
1. Download Open buildings 2D data from google.  
2. Run "open_buildings/preprocessing.ipynb" on that. It will create a single scene based on that buildings data and write the scene on a file.  
    ### Format:
        a scene_id polygon_id vertex_id x y  
        .....  
        b scene_minX scene_maxX scene_minY scene_maxY
    ### Example:
    	Say a scene has one only triangle. Then the written file will be kind of -  
    	a 0 0 0 5 6  
    	a 0 0 1 3 4  
    	a 0 0 2 2 3  
    	b 0 10 0 10  
3. Run "data_gen/Polygon/gen_scene.cpp" on any scene file to create sub scenes based on random sampling or grid sampling (you choose) and write them to a file.  
4. Run "data_gen/Polygon/select_scene.cpp" on any scene file to select scenes based on area coverage and write them to a file.  
5. Run "data_gen/Polygon/polygon_gen.cpp" on any scene file to generate dataset for training (encoding, query points).  


## Visualization
1. Run "visualization/visualize_polygon_scene.ipynb" on a scene file to visualizes it.  
    ### Controls:  
        + ---> zooms in  
        - ---> zooms out  
        mouse left drag ---> moves the scene  
        ....  
        Actually it has a lot of control like generate encoding of current screen, predict, fit selected area to screen etc. You should check the code to find about them.  
2. Run "visualization/visualize_dataset.ipynb" to visualize any dataset file.  


## Bugs:  
1. There is a bug when creating encoding for polygon. It is a special case.
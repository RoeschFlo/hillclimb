#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <vector>
#include <list>


typedef struct Projectile {
    Vector3 position;
    Vector3 velocity;
    float lifeTime;
    float radius;
    Color color;
    bool active;
} Projectile;

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    // Fenster erstellen
    InitWindow(screenWidth, screenHeight, "Mein erstes Raylib-Spiel");

    DisableCursor(); // Mauszeiger deaktivieren

    Camera3D camera = { 0 };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    const float terrainWorldWidth = 500.0f;
    const float terrainWorldDepth = 500.0f;
    const float terrainMaxHeight = 20.0f;
    const int terrainImageWidth = 200;
    const int terrainImageHeight = 200;

    Image heightmapImage  = GenImageCellular (terrainImageWidth, terrainImageHeight, 18); // Heightmap generieren

    Mesh terrainMesh = GenMeshHeightmap(heightmapImage, {terrainWorldWidth, terrainMaxHeight, terrainWorldDepth}); // Mesh aus Heightmap generieren

    UnloadImage(heightmapImage); // Heightmap-Image nicht mehr benötigt
    Model terrainModel = LoadModelFromMesh(terrainMesh); // Modell aus Mesh generieren

    terrainModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = GRAY; // Terrain-Farbe setzen

    Mesh cubeMesh = GenMeshCube(2.0f, 2.0f, 2.0f); // Würfel-Mesh generieren

    Model shipModel = LoadModelFromMesh(cubeMesh); // Modell aus Würfel-Mesh generieren
    shipModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = RED; // Schiff-Farbe setzen
    Vector3 shipPosition = { terrainWorldWidth/2.0f, terrainMaxHeight + 10.0f, terrainWorldDepth/2.0f }; // Startposition des Schiffs
    float shipYaw = 0.0f; // Start-Rotation des Schiffs
    float shipPitch = 0.0f;

    const float mouseSensitivity = 0.003f;
    const float shipSpeed = 25.0f;

    std::list<Projectile> projectiles; // Liste der aktiven Projektile
    const int MAX_PROJECTILES = 50; // Maximale Anzahl an Projektile

    const float PROJECTILE_SPEED = 50.0f;
    const float PROJECTILE_LIFETIME = 5.0f;
    const float PROJECTILE_RADIUS = 0.2f;
    Color PROJECTILE_COLOR = BLUE;

    float shootCooldown = 0.0f;
    const float SHOOT_RATE = 0.2f; // Zeit zwischen den Schüssen in Sekunden

    SetTargetFPS(60); // Ziel-FPS setzen





    // Haupt-Game-Loop
    while (!WindowShouldClose()) {

        float dt = GetFrameTime();

        Vector2 mouseDelta = GetMouseDelta();
        shipYaw -= mouseDelta.x * mouseSensitivity;
        shipPitch -= mouseDelta.y * mouseSensitivity;

        shipPitch = Clamp(shipPitch, -PI/2.0f-0.01f, PI/2.0f-0.01f);

        //first rotate around Y axis (yaw), then around X axis (pitch)
        Matrix shipRotationMatrix = MatrixMultiply(MatrixRotateY(shipYaw), MatrixRotateX(shipPitch));
        //alternative if roll was needed:
        //Matrix shipRotationMatrix = MatrixMultiply(MatrixMultiply(MatrixRotateZ(shipRoll), MatrixRotateX(shipPitch)), MatrixRotateY(shipYaw));

        Vector3 shipForward = Vector3Transform((Vector3){0,0,1}, shipRotationMatrix);
        Vector3 shipRight = Vector3Transform((Vector3){1,0,0},shipRotationMatrix);
        Vector3 shipUp = Vector3Transform((Vector3){0,1,0}, shipRotationMatrix);

        // Bewegung des Schiffs
        Vector3 moveDirection = {0};
        if (IsKeyDown(KEY_W)) moveDirection = Vector3Add(moveDirection, shipForward);
        if (IsKeyDown(KEY_S)) moveDirection = Vector3Subtract(moveDirection,    shipForward);
        if (IsKeyDown(KEY_A)) moveDirection = Vector3Subtract(moveDirection, shipRight);
        if (IsKeyDown(KEY_D)) moveDirection = Vector3Add(moveDirection, shipRight);

        if(IsKeyDown(KEY_SPACE)||IsKeyDown(KEY_E)) moveDirection.y += 1.0f;
        if(IsKeyDown(KEY_LEFT_CONTROL)||IsKeyDown(KEY_Q)) moveDirection.y -= 1.0f;

        if (Vector3LengthSqr(moveDirection)> 0.001f)
        {   
            moveDirection = Vector3Normalize(moveDirection);
            shipPosition = Vector3Add(shipPosition, Vector3Scale(moveDirection, shipSpeed * dt));
        }

        if (shootCooldown > 0.0f)
            shootCooldown -= dt;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && shootCooldown <= 0.0f )
        {  
            if(projectiles.size() < MAX_PROJECTILES)
            {
                Projectile newProjectile;
                newProjectile.position = Vector3Add(shipPosition, Vector3Scale(shipForward, 1.1f)); // Start etwas vor dem Schiff
                newProjectile.velocity = Vector3Scale(shipForward, PROJECTILE_SPEED);
                newProjectile.lifeTime = PROJECTILE_LIFETIME;
                newProjectile.radius = PROJECTILE_RADIUS;
                newProjectile.color = PROJECTILE_COLOR;
                newProjectile.active = true;

                projectiles.push_back(newProjectile);
                shootCooldown = SHOOT_RATE;
            }
        }

        // Update der Projektile
        for (auto it = projectiles.begin(); it != projectiles.end(); )
        {
            if (it->active)
            {   
                it->position = Vector3Add(it->position, Vector3Scale(it->velocity, dt));
                it->lifeTime -= dt;

                if (it->lifeTime <= 0.0f)
                {
                    it->active = false;
                }
                //Todo: Kollision mit Terrain prüfen
                
                if(!it->active){
                    it = projectiles.erase(it);
                } else {
                    ++it;
                }
            }
        }   

        float cameraDistance = 15.0f;
        float camerHeightOffset = 5.0f;

        Vector3 cameraOffset = Vector3Scale(shipForward, -cameraDistance);
        
        cameraOffset.y += camerHeightOffset;

        camera.position = Vector3Add(shipPosition, cameraOffset);
        camera.target = shipPosition;
        camera.up = {0,1,0 };






        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(camera);
        DrawModelWires(terrainModel, (Vector3){0,0,0}, 1.0f, DARKGRAY); // Terrain zeichnen
        DrawModel(terrainModel, (Vector3){0,0,0}, 1.0f, WHITE); // Terrain zeichnen

        shipModel.transform = MatrixMultiply(shipRotationMatrix,MatrixTranslate(shipPosition.x, shipPosition.y, shipPosition.z));

        DrawModel(shipModel, (Vector3){0,0,0}, 1.0f, WHITE); // Schiff zeichnen
        // Projektile zeichnen
        for (const auto& proj : projectiles)
        {
            if (proj.active)
            {
                DrawSphere(proj.position, proj.radius, proj.color);
            }
        }

        //DrawHelperGrid 
        //Draws  a 3d grid  on the xz olane for spation reference
        DrawGrid(100, 10.0f);
        EndMode3D();

        DrawText("Spaceship Demo -W,A,S,D, Mouse, LMB to Shoot", 10, 10, 20, LIGHTGRAY);
        DrawText(TextFormat("Ship Position: [%.2f, %.2f, %.2f]", shipPosition.x, shipPosition.y, shipPosition.z), 10, 25, 20, LIGHTGRAY);
        DrawText(TextFormat("Projectiles: %d", (int)projectiles.size()), 10, 40, 20, LIGHTGRAY);
        DrawText(TextFormat("Ship Yaw: %.2f Pitch: %.2f", shipYaw, shipPitch), 10, 70, 20, LIGHTGRAY);
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 55, 20, LIGHTGRAY);

        EndDrawing();
    }

    UnloadModel(terrainModel); // Modell entladen
    UnloadModel(shipModel);    // Modell entladen
    //if you loaded any other resources, unload them here Unloadtexture(yourTexture), UnloadSound(yourSound),
    //ClodeAudioDevice etc.


    CloseWindow(); // Fenster schließen
    return 0;
}

/*
Für die Welt wird ein Proxy-Entwursmuster verwendet. Es ist nie die ganze Welt sichtbar. Es wird immer nur ein Ausschnitt der Welt dargestellt.
Der Proxy sorgt dafür, dass nur die sichtbaren Teile der Welt geladen werden und die nicht sichtbaren Teile ausgelagert werden.
Dies spart Speicher und Rechenleistung.

Das Singleton-Muster wird für die Kamerasteuerung verwendet. Es gibt nur eine Kamera im Spiel, die von verschiedenen Teilen des Codes verwendet wird.
-------------------------------------------------------------------------------------------
🎮 1. Entity–Component–System (ECS)

Kategorie: Architektur-Pattern
Warum?

Sehr flexibel

Vermeidet tiefe Klassenhierarchien

Komponenten definieren Verhalten (Transform, MeshRenderer, PhysicsBody, Sound…)

Systeme verarbeiten viele Entities extrem effizient (Cache-freundlich)

Beispiel:struct Transform { Vector3 pos; Quaternion rot; };
struct MeshRenderer { Mesh mesh; Material mat; };

class RenderSystem {
public:
    void update() {
        for (auto e : entitiesWith<Transform, MeshRenderer>()) {
            drawMesh(e.get<MeshRenderer>(), e.get<Transform>());
        }
    }
};
-------------------------------------------------------------------------------------------
🎮 2. Singleton (sparsam!)

Kategorie: GOF Creational
Warum?
Manchmal sinnvoll für:

Renderer/GraphicsDevice

AudioManager

Input

AssetManager

GameState
-------------------------------------------------------------------------------------------
🎮 3. Factory / Abstract Factory

Warum?
Zum Erstellen von:

Gegnern

Projektile

Levels

Items

Partikelsystemen

UI-Elementen

Ohne jede Erstellen-Logik hart zu verdrahten.
-------------------------------------------------------------------------------------------
🎮 4. Observer / EventBus

Warum?
Lose Kopplung zwischen Systemen.
Perfekt für:

Kollisionen

„Player died“

„Ammo picked up“

UI Updates

Achievements

Quests
-------------------------------------------------------------------------------------------
🎮 5. Command Pattern

Warum?
Ideal für Input-Mapping, Undo/Redo, Replay/Record, Konsolenbefehle.

-------------------------------------------------------------------------------------------
🎮 6. State Pattern

Warum?
Für komplexe Zustände:

Player: Idle, Walk, Run, Jump, Fall, Attack

Gegner-KI

Waffen

GameState (MainMenu, Loading, InGame, Pause)
-------------------------------------------------------------------------------------------
🎮 7. Strategy Pattern

Warum?
Unterschiedliche Algorithmen austauschbar machen:

Pathfinding-Strategien

Steering Behavior

Shader-Strategien

LOD-Strategien
-------------------------------------------------------------------------------------------
8. Flyweight Pattern

Warum?
Speichern vieler kleiner Objekte mit geteilten Daten:
Bäume, Gras, Partikel, Projektil-Meshes, Fonts.

Beispiel:
Ein Baum besteht aus:

Position = unique

Mesh & Textur = shared

→ spart extrem viel RAM/GPU Speicher.
-------------------------------------------------------------------------------------------
🎮 9. Component Pattern (nicht ECS)

Warum?
Auch ohne ECS kann man GameObjects in Komponenten zerlegen:

TransformComponent

RenderComponent

PhysicsComponent
-------------------------------------------------------------------------------------------
🎮 10. Service Locator

Warum?
Erlaubt globale Services ohne echte Singletons.
Wird oft in Engines eingesetzt (z. B. Audio).
-------------------------------------------------------------------------------------------
11. Decorator

Warum?
Zur Laufzeit Eigenschaften erweitern:

Buffs/Debuffs

Power-Ups

Waffenerweiterungen

Perks
-------------------------------------------------------------------------------------------
🎮 12. Prototype

Warum?
Instanziieren neuer Objekte durch Kopieren eines „Blueprints“ (z. B. Prefabs).
Welche davon sind besonders wichtig für ein 3D-Spiel?

✅ ECS (oder Component-based)
✅ State
✅ Observer/EventBus
✅ Factory/Prototype
*/
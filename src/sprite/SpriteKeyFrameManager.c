/**
 * aseprite 1.2.28
 * parse json using cjon. xxxx.json and xxxx.png where xxxx is the name of both exported json/png
 */

/*
{ "frames": {
   "mario_small (idle) 0.ase": {
    "frame": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (idle) 1.ase": {
    "frame": { "x": 16, "y": 0, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (idle) 2.ase": {
    "frame": { "x": 32, "y": 0, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (idle) 3.ase": {
    "frame": { "x": 48, "y": 0, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (idle) 4.ase": {
    "frame": { "x": 64, "y": 0, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (walk) 0.ase": {
    "frame": { "x": 0, "y": 16, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (walk) 1.ase": {
    "frame": { "x": 16, "y": 16, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (walk) 2.ase": {
    "frame": { "x": 32, "y": 16, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (walk) 3.ase": {
    "frame": { "x": 48, "y": 16, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (walk) 4.ase": {
    "frame": { "x": 64, "y": 16, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (break) 0.ase": {
    "frame": { "x": 0, "y": 32, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (break) 1.ase": {
    "frame": { "x": 16, "y": 32, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (break) 2.ase": {
    "frame": { "x": 32, "y": 32, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (break) 3.ase": {
    "frame": { "x": 48, "y": 32, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (break) 4.ase": {
    "frame": { "x": 64, "y": 32, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (jump) 0.ase": {
    "frame": { "x": 0, "y": 48, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (jump) 1.ase": {
    "frame": { "x": 16, "y": 48, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (jump) 2.ase": {
    "frame": { "x": 32, "y": 48, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (jump) 3.ase": {
    "frame": { "x": 48, "y": 48, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (jump) 4.ase": {
    "frame": { "x": 64, "y": 48, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (die) 0.ase": {
    "frame": { "x": 0, "y": 64, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (die) 1.ase": {
    "frame": { "x": 16, "y": 64, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (die) 2.ase": {
    "frame": { "x": 32, "y": 64, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (die) 3.ase": {
    "frame": { "x": 48, "y": 64, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (die) 4.ase": {
    "frame": { "x": 64, "y": 64, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (crush) 0.ase": {
    "frame": { "x": 0, "y": 80, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (crush) 1.ase": {
    "frame": { "x": 16, "y": 80, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (crush) 2.ase": {
    "frame": { "x": 32, "y": 80, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (crush) 3.ase": {
    "frame": { "x": 48, "y": 80, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (crush) 4.ase": {
    "frame": { "x": 64, "y": 80, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (swimming) 0.ase": {
    "frame": { "x": 0, "y": 96, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (swimming) 1.ase": {
    "frame": { "x": 16, "y": 96, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (swimming) 2.ase": {
    "frame": { "x": 32, "y": 96, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (swimming) 3.ase": {
    "frame": { "x": 48, "y": 96, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   },
   "mario_small (swimming) 4.ase": {
    "frame": { "x": 64, "y": 96, "w": 16, "h": 16 },
    "rotated": false,
    "trimmed": false,
    "spriteSourceSize": { "x": 0, "y": 0, "w": 16, "h": 16 },
    "sourceSize": { "w": 16, "h": 16 },
    "duration": 100
   }
 },
 "meta": {
  "app": "http://www.aseprite.org/",
  "version": "1.2.28-x64",
  "format": "RGBA8888",
  "size": { "w": 80, "h": 112 },
  "scale": "1",
  "frameTags": [
  ],
  "layers": [
   { "name": "idle", "opacity": 255, "blendMode": "normal" },
   { "name": "walk", "opacity": 255, "blendMode": "normal" },
   { "name": "break", "opacity": 255, "blendMode": "normal" },
   { "name": "jump", "opacity": 255, "blendMode": "normal" },
   { "name": "die", "opacity": 255, "blendMode": "normal" },
   { "name": "crush", "opacity": 255, "blendMode": "normal" },
   { "name": "swimming", "opacity": 255, "blendMode": "normal", "cels": [{ "frame": 4, "data": "hola que tal" }] }
  ],
  "slices": [
  ]
 }
}
*/

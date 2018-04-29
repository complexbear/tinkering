
var scene, camera;

document.addEventListener("DOMContentLoaded", function () {

    var canvas = document.getElementById("canvas"),
        lights, renderer,
        windowSize = 500,
        meshes = [];

    function initScene() {

        scene = new THREE.Scene();
        camera = new THREE.PerspectiveCamera(75, windowSize / windowSize, 0.1, 200);
        camera.position.z = 30;
        camera.position.y = 10;

        renderer = new THREE.WebGLRenderer({
            antialias: true,
            canvas: canvas
        });
        renderer.setPixelRatio(window.devicePixelRatio);
        renderer.setSize(windowSize, windowSize);
        renderer.setClearColor(0x000000, 1);


        lights = [];
        lights[0] = new THREE.PointLight(0xffffff, 1, 0);
        lights[1] = new THREE.PointLight(0xffffff, 1, 0);
        lights[2] = new THREE.PointLight(0xffffff, 1, 0);

        lights[0].position.set(0, 200, 0);
        lights[1].position.set(100, 200, 100);
        lights[2].position.set(- 100, - 200, - 100);

        scene.add(lights[0]);
        scene.add(lights[1]);
        scene.add(lights[2]);

    }

    function render() {
        requestAnimationFrame(render);
        renderer.render(scene, camera);
    }

    initScene();

    render();
});

clearScene = function () {
    console.info('Removing all children...')
    for (let i = scene.children.length - 1; i >= 0; i--) {
        let child = scene.children[i];

        if (child !== camera) { // plane & camera are stored earlier
            scene.remove(child);
        }
    }

}

LSystemRender = function () {
    clearScene();

    // Get l-system document
    var  doc = document.getElementById("lsystem_document").textContent;
    console.log("doc: " + doc);
        
    var material = new THREE.LineBasicMaterial({ color: 0x0000ff }),
        lsys = new LSystem(doc, material);

    lsys.draw(scene);
}




LSystemRender = function() {
    // Get l-system document
    var canvas = document.getElementById("canvas"),
        renderArea = document.getElementById("renderArea"),
        doc = document.getElementById("lsystem_document").textContent;
    console.log("doc: " + doc);

    var scene, camera, renderer,
        lights, helpers = [],
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

        initBones(doc, 0);
        initBones(doc, 20);
    }

    function createGeometry(sizing) {

        var geometry = new THREE.CylinderGeometry(
            5,                       // radiusTop
            5,                       // radiusBottom
            sizing.height,           // height
            8,                       // radiusSegments
            sizing.segmentCount,     // heightSegments
            true                     // openEnded
        );

        for (var i = 0; i < geometry.vertices.length; i++) {

            var vertex = geometry.vertices[i];
            var y = (vertex.y + sizing.halfHeight);

            var skinIndex = Math.floor(y / sizing.segmentHeight);
            var skinWeight = (y % sizing.segmentHeight) / sizing.segmentHeight;

            geometry.skinIndices.push(new THREE.Vector4(skinIndex, skinIndex + 1, 0, 0));
            geometry.skinWeights.push(new THREE.Vector4(1 - skinWeight, skinWeight, 0, 0));

        }

        return geometry;

    }

    

    function createMesh(geometry, bones, idx) {
        var c = 0x156289;
        if (idx > 1) {
            c = 0xAA0000;
        }

        var material = new THREE.MeshPhongMaterial({
            skinning: true,
            color: c,
            emissive: 0x072534,
            side: THREE.DoubleSide,
            shading: THREE.FlatShading
        });

        var mesh = new THREE.SkinnedMesh(geometry, material);
        var skeleton = new THREE.Skeleton(bones);

        if (idx > 1) {
            mesh.position.x = 2;
        }

        mesh.add(bones[0]);
        mesh.bind(skeleton);

        var skeletonHelper = new THREE.SkeletonHelper(mesh);
        skeletonHelper.material.linewidth = 2;
        scene.add(skeletonHelper);
        helpers.push(skeletonHelper);

        return mesh;

    }
    
    function initBones(doc, idx) {
        var segmentHeight = 4;
        var segmentCount = doc.length;
        var height = segmentHeight * segmentCount;
        var halfHeight = height * 0.5;

        var sizing = {
            segmentHeight: segmentHeight,
            segmentCount: segmentCount,
            height: height,
            halfHeight: halfHeight
        };

        var geometry = createGeometry(sizing);
        var bones = createTree(doc, sizing, idx);
        var mesh = createMesh(geometry, bones, idx);

        mesh.scale.multiplyScalar(1);
        scene.add(mesh);
        meshes.push(mesh);
    }

    function render() {

        requestAnimationFrame(render);
        var time = Date.now() * 0.001;
        //Wiggle the bones
        if (true /*state.animateBones*/) {
            for (var m = 0; m < meshes.length; ++m) {
                for (var i = 0; i < meshes[m].skeleton.bones.length; i++) {
                    meshes[m].skeleton.bones[i].rotation.z = Math.sin(time) * 2 / meshes[m].skeleton.bones.length;
                }
            }
        }

        for (var i = 0; i < helpers.length; ++i) {
            helpers[i].update();
        }
        renderer.render(scene, camera);
    }

    initScene();
    render();
}

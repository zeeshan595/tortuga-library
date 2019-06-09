const gulp = require("gulp");
const walk = require("walk");
const shell = require("child_process");
const path = require("path");

const config = {
  projects: [
    {
      name: "Tortuga",
      type: "Library",
      main: "Tortuga.h",
      flags: "-Wall -g -std=c++17 -Wnarrowing -DDEBUG_MODE",
      compiler: "g++",
      input: "Source",
      output: "Build",
    },
    {
      name: "Editor",
      type: "Console",
      main: "Application.cpp",
      flags: "-Wall -g -std=c++17 -Wnarrowing -DDEBUG_MODE -pthread -lvulkan -lSDL2 -ldl -lsndio -lSDL2_image",
      compiler: "g++",
      input: "Source",
      output: "Build",
      includes: "-ITortuga/Source/",
      libs: "Tortuga/Build/libTortuga.a",
    }
  ]
};

gulp.task("clean", gulp.series(() => {
  return exec("rm -r Tortuga/Build");
}, () => {
  return exec("rm -r Editor/Build");
}));

gulp.task("build", gulp.series(async () => {
  for (let i = 0; i < config.projects.length; i++) {
    const project = config.projects[i];
    console.log(`Compiling project ${project.name}`);
    const files = (await getRecursiveFiles(`${project.name}/${project.input}`)).
      filter((val) => val.pathWithExt != `${project.name}/${project.input}/${project.main}`);
    const compiledFilesPromise = [];
    for (let j = 0; j < files.length; j++) {
      compiledFilesPromise.push(compileFile(project, files[j]));
    }
    const compiledFiles = await Promise.all(compiledFilesPromise);
    await compileMainFile(project, compiledFiles);
  }
}));

const compileMainFile = async (project, compiledFiles) => {
  if (project.type == "Library") {
    console.log(`Building: ${project.name}/${project.output}/lib${project.name}.a`);
    await exec(`ar rcs ${project.name}/${project.output}/lib${project.name}.a ${compiledFiles.join(' ')}`);
  } else if (project.type == "Console") {
    console.log(`Building ${project.name}/${project.output}/${project.main.replace(/.[a-z]{1,4}$/g, '')}`);
    exec(
      `${project.compiler} ${project.name}/${project.input}/${project.main} ` +
      `-o ${project.name}/${project.output}/${project.main.replace(/.[a-z]{1,4}$/g, '')} ` +
      `${project.includes} ` +
      `${project.libs} ` +
      `${project.flags} `
    );
  }
}

const compileFile = async (project, input) => {
  const compiledPath = input.pathWithoutExt.replace(`${project.name}/${project.input}`, `${project.name}/${project.output}`);

  if (/.(cpp)$/g.test(input.pathWithExt)) {
    console.log(`${input.pathWithExt} -> ${compiledPath}.o`);
    await exec(`mkdir -p "${path.dirname(compiledPath)}"`);
    await exec(`${project.compiler} -c ${input.pathWithExt} -o ${compiledPath}.o ${project.flags}`);
    return `${compiledPath}.o`;
  } else if (/.(comp)$/g.test(input.pathWithExt)) {
    console.log(`${input.pathWithExt} -> ${compiledPath}.spv`);
    await exec(`mkdir -p "${path.dirname(compiledPath)}"`);
    await exec(`glslangValidator -V ${input.pathWithExt} -o ${compiledPath}.spv`);
    return `${compiledPath}.spv`;
  }

  return null;
}

const exec = async (command) => {
  return new Promise((resolve, reject) => {
    shell.exec(command, (err, stdout, stderr) => {
      if (!err)
        resolve(stdout);
      else {
        console.log(command);
        console.error(stderr);
        resolve(stderr);
      }
    });
  });
}

const getRecursiveFiles = async (filePath) => {
  const files = [];
  return new Promise((resolve, reject) => {
    const walker = walk.walk(filePath, { followLinks: false });
    walker.on('file', function (root, stat, next) {
      const file = root + '/' + stat.name;
      files.push({
        dir: path.dirname(file),
        pathWithExt: file,
        pathWithoutExt: file.replace(/(.[a-z]{1,4})$/g, "")
      });
      next();
    });

    walker.on('end', function () {
      resolve(files);
    });
  })
}
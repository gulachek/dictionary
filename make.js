const { Command } = require('commander');
const { CppBuildCommand } = require('gulpachek/cpp');
const { spawn } = require('child_process');
const { series } = require('bach');

const cmd = new Command();
const cppBuild = new CppBuildCommand({
	program: cmd,
	cppVersion: 20
});

function buildLib(args) {
	const { cpp } = args;

	const dict = cpp.compile({
		name: 'com.gulachek.dictionary',
		version: '0.1.0',
		apiDef: 'GULACHEK_DICTIONARY_API'
	});

	dict.include('include');

	const gtree = cpp.require('com.gulachek.gtree', '0.1.0');

	dict.link(gtree);

	return dict;
}

const test = cmd.command('test')
.description('unit tests');

cppBuild.configure(test, (args) => {
	const { cpp, sys } = args;

	const dict = buildLib(args);
	const boostTest = cpp.require('org.boost.unit-test-framework', '1.80.0');

	const test = cpp.compile({
		name: 'dict_test',
		src: [ 'test/dict_test.cpp' ]
	});

	test.link(dict);
	test.link(boostTest);

	const exe = test.executable();

	return series(sys.rule(exe), () => spawn(exe.abs(), { stdio: 'inherit' }));
});

cppBuild.pack(buildLib);

cmd.parse();

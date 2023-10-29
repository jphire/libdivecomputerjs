global.Buffer = require('buffer/').Buffer;
var libdivecomputer = require('bindings')('libdivecomputerjs.node');
module.exports = libdivecomputer;
package main

import (
	"flag"
	"fmt"
	"os"
	"strings"

	"github.com/ProtossGenius/SureMoonNet/basis/smn_file"
	"github.com/ProtossGenius/smntools/smnt/codedeal"
	"go4.org/sort"
)

func check(err error) {
	if err != nil {
		panic(err)
	}
}

func cpath(p string) string {
	p = strings.TrimPrefix(p, "./")

	for strings.Contains(p, "//") {
		p = strings.ReplaceAll(p, "//", "/")
	}

	return p
}

func main() {
	actions := sort.StringSlice{}
	pathList := []string{}
	target := flag.String("target", "./actions", "action dir.")
	flag.Parse()

	hFile, err := smn_file.CreateNewFile("./actions.h")
	check(err)
	srcFile, err := smn_file.CreateNewFile("./actions.cpp")
	check(err)

	defer hFile.Close()
	defer srcFile.Close()

	hwritef := func(f string, a ...interface{}) {
		_, err = hFile.WriteString(fmt.Sprintf(f+"\n", a...))
		check(err)
	}

	cwritef := func(f string, a ...interface{}) {
		_, err = srcFile.WriteString(fmt.Sprintf(f+"\n", a...))
		check(err)
	}

	_, err = smn_file.DeepTraversalDir(*target, func(path string, info os.FileInfo) smn_file.FileDoFuncResult {
		if info.IsDir() {
			return smn_file.FILE_DO_FUNC_RESULT_DEFAULT
		}

		if !strings.HasSuffix(info.Name(), ".h") {
			return smn_file.FILE_DO_FUNC_RESULT_DEFAULT
		}

		pathList = append(pathList, cpath(path))
		data, err := smn_file.FileReadAll(path)
		check(err)
		code, err := codedeal.DeleteComment(string(data))
		check(err)
		for _, line := range strings.Split(code, "\n") {
			line = strings.TrimSpace(line)

			if strings.HasPrefix(line, "dtaction") && strings.Contains(line, ",") && strings.Contains(line, "(") &&
				strings.Index(line, "(") < strings.Index(line, ",") {
				actionName := strings.TrimSpace(line[strings.Index(line, "(")+1 : strings.Index(line, ",")])
				actions = append(actions, actionName)
			}
		}
		return smn_file.FILE_DO_FUNC_RESULT_DEFAULT
	})
	check(err)

	sort.Sort(actions)

	hwritef(`#ifndef ACTIONS_H_TV3HQ2CU
#define ACTIONS_H_TV3HQ2CU
//Product by github.com/ProtossGenius/DynamicTest/cmd/smdtcact, should not change it.
`)
	defer hwritef(`#endif /* end of include guard: ACTIONS_H_TV3HQ2CU */`)

	hwritef("#include <string>")

	for _, h := range pathList {
		hwritef(`#include "%s"`, h)
	}

	cwritef(`#include "actions.h"`)

	hwritef("namespace smdtest{")
	cwritef("namespace smdtest{")

	defer cwritef("} // smdtest")
	defer hwritef("} //smdtest")

	for _, act := range actions {
		hwritef("\textern const std::string Name%s;", act)
		cwritef("\tconst std::string Name%s(%s::Name());", act, act)
	}
}

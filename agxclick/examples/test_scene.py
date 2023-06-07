from scene import extract_file_model


class TestArgumentParser:
    def test_that_linux_path_works(self):
        file, model = extract_file_model("/top/model.yml:modelname")
        assert file == "/top/model.yml"
        assert model == "modelname"

    def test_that_windows_path_works(self):
        file, model = extract_file_model("c:\\top\\model.yml:modelname")
        assert file == "c:\\top\\model.yml"
        assert model == "modelname"

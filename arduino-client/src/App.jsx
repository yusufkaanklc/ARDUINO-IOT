import {
  Box,
  Flex,
  Heading,
  Text,
  Button,
  Center,
  Divider,
} from "@chakra-ui/react";
import { useEffect, useState } from "react";
import axios from "axios";
function App() {
  const [lightStateOne, setLightStateOne] = useState(false);
  const [lightStateTwo, setLightStateTwo] = useState(false);
  const [LDROne, setLDROne] = useState(false);
  const [LDRTwo, setLDRTwo] = useState(false);
  const getState = async () => {
    try {
      const fetch = await axios.get("/get-state");
      setLightStateOne(fetch.data.lightState1);
      setLightStateTwo(fetch.data.lightState2);
      setLDROne(fetch.data.ldr1);
      setLDRTwo(fetch.data.ldr2);
    } catch (error) {
      console.log(error);
    }
  };

  const updateState = async (state) => {
    try {
      await axios.post("/update-state", state);
      await getState();
    } catch (error) {
      console.log(error);
    }
  };

  useEffect(() => {
    getState();
  }, []);

  return (
    <>
      <Box bgColor={"#1a202c"} h={"100vH"}>
        <Center>
          <Heading color={"white"} p={"1rem"} mb={"10rem"}>
            Akıllı Enerji Yönetimi Sistemi
          </Heading>
        </Center>
        <Flex align={"center"} gap={"5rem"} justify={"center"}>
          <Flex
            flexDir={"column"}
            px={"1rem"}
            gap={"1rem"}
            align={"center"}
            h={"15rem"}
          >
            <Box w={"max-content"}>
              <Text color={"white"} pr={"1rem"}>
                Led yönetimi
              </Text>
              <Divider my={"1rem"}></Divider>
              <Flex flexDir={"column"} gap={"1rem"}>
                <Box>
                  <Text color={"white"} mb={".5rem"}>
                    Led1
                  </Text>
                  <Button
                    onClick={() => updateState({ lightState1: !lightStateOne })}
                  >
                    {lightStateOne === true ? "Açık" : "Kapalı"}
                  </Button>
                </Box>
                <Box>
                  <Text color={"white"} mb={".5rem"}>
                    Led2
                  </Text>
                  <Button
                    onClick={() => updateState({ lightState2: !lightStateTwo })}
                  >
                    {lightStateTwo === true ? "Açık" : "Kapalı"}
                  </Button>
                </Box>
              </Flex>
            </Box>
          </Flex>
          <Flex
            flexDir={"column"}
            px={"1rem"}
            gap={"1rem"}
            align={"center"}
            h={"15rem"}
          >
            <Box w={"max-content"}>
              <Text color={"white"} pr={"1rem"}>
                Işık kaynağı durumu
              </Text>
              <Divider my={"1rem"}></Divider>
              <Flex flexDir={"column"} gap={"1rem"}>
                <Flex flexDir={"column"} gap={".5rem"}>
                  <Text color={"white"}>{`Sensör1: ${LDROne}`}</Text>
                </Flex>
                <Flex flexDir={"column"} gap={".5rem"}>
                  <Text color={"white"}>{`Sensör2: ${LDRTwo}`}</Text>
                </Flex>
              </Flex>
            </Box>
          </Flex>
        </Flex>
      </Box>
    </>
  );
}

export default App;
